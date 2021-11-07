#include "softmbesession.hpp"
#include "interleaver_permutations_const.hpp"
#include <cstring>

// Our audio buffers are 2*160 bytes long since we have 50 packets/sec; 8000/50=160 and a short has two bytes
#define AUDIO_BUFFER_SIZE 320

using namespace softmbe;

softmbeSession::softmbeSession(int q)
{
    uvquality = q;
    audio_queue = new BlockingQueue<short>(1000);
    mbe_initMbeParms(&cur_mp, &prev_mp, &prev_mp_enhanced);
}

void softmbeSession::encode(char* input, size_t size)
{
    std::cout << "Error: encode() method was called (unsupported)" << std::endl;
}

void softmbeSession::decode(char* input, size_t size)
{
    // Call vocoder-specific decode method which do the conversion
    // and append the result to the audio_queue.
    switch(vocoder)
    {
        case VOCODER_AMBE_3600x2450:
            decode_ambe_3600x2450(input, size);
            break;
        case VOCODER_AMBE_3600x2400:
            decode_ambe_3600x2400(input, size);
            break;
        case VOCODER_AMBE_2450:
            decode_ambe_2450(input, size);
            break;
        case VOCODER_IMBE_4400:
            decode_imbe_4400(input, size);
            break;
    }
}

void softmbeSession::decode_ambe_3600x2450(char* input, size_t size)
{
    if(size != 9)
    {
        std::cout << "MBE buffer has " << size << " bytes but should be 9." << std::endl;
        return;
    }

    // First we unpack to bits
    char mbe_packet_interleaved[72];
    for(int i=0; i<72; i++) mbe_packet_interleaved[i] = (input[i/8] >> (7-i%8)) & 0x01;

    // dmr_decoder produces packets that are interleaved. We therefore have to de-interleave
    // using the appropriate permutation table.
    char mbe_packet_deinterleaved[72];
    for(int i=0; i<72; i++) mbe_packet_deinterleaved[i] = mbe_packet_interleaved[dmr_interleave_permutation[i]];

    // Then we populate the AMBE frames
    char ambe_fr[4][24] = { 0 };
    for(int i=  0,j=0; i< 24; i++,j++) ambe_fr[0][j] = mbe_packet_deinterleaved[i]; // c0
    for(int i= 24,j=0; i< 47; i++,j++) ambe_fr[1][j] = mbe_packet_deinterleaved[i]; // c1
    for(int i= 47,j=0; i< 58; i++,j++) ambe_fr[2][j] = mbe_packet_deinterleaved[i]; // c2
    for(int i= 58,j=0; i< 72; i++,j++) ambe_fr[3][j] = mbe_packet_deinterleaved[i]; // c3

    int errs = 0;
    int errs2 = 0;
    char err_str[64];

    short* audio_out_buf = (short*)std::malloc(AUDIO_BUFFER_SIZE);
    char ambe_d[49] = { 0 };
    mbe_processAmbe3600x2450Frame(audio_out_buf, &errs, &errs2, err_str, ambe_fr, ambe_d, &cur_mp, &prev_mp, &prev_mp_enhanced, uvquality);
    audio_queue->push(audio_out_buf, false);
}

void softmbeSession::decode_ambe_3600x2400(char* input, size_t size)
{
    if(size != 9)
    {
        std::cout << "MBE buffer has " << size << " bytes but should be 9." << std::endl;
        return;
    }

    // First we unpack to bits here. Note that we unpack LSB first which is unusual.
    char mbe_packet_interleaved[72];
    for(int i=0; i<72; i++) mbe_packet_interleaved[i] = (input[i/8] >> (i%8)) & 0x01;

    // dstar_decoder delivers a packet that has not yet been deinterleaved. We
    // use the permutation table to de-interleave straight into the ambe_fr
    // data structure. We could do all of this in one go but meh...
    char ambe_fr[4][24] = { 0 };
    const int *v = dstar_decoder_vector_permutation, *b = dstar_decoder_bit_permutation;
    for(int i=0; i < 72; i++, v++, b++)
    {
        ambe_fr[*v][*b] = mbe_packet_interleaved[i];
    }

    int errs = 0;
    int errs2 = 0;
    char err_str[64];

    short* audio_out_buf = (short*)std::malloc(AUDIO_BUFFER_SIZE);
    char ambe_d[49] = { 0 };
    mbe_processAmbe3600x2400Frame(audio_out_buf, &errs, &errs2, err_str, ambe_fr, ambe_d, &cur_mp, &prev_mp, &prev_mp_enhanced, uvquality);
    audio_queue->push(audio_out_buf, false);
}

void softmbeSession::decode_ambe_2450(char* input, size_t size)
{
    if(size != 7)
    {
        std::cout << "MBE buffer has " << size << " bytes but should be 7." << std::endl;
        return;
    }
    char mbe_packet_interleaved[49];

    // First we unpack to bits
    for(int i=0; i<49; i++) mbe_packet_interleaved[i] = (input[i/8] >> (7-i%8)) & 0x01;

    // However, ysf_decoder delivers an interleaved packet according to the
    // permutation documented here: https://github.com/HB9UF/gr-ysf/issues/12
    // We need to undo this interleaving to process with libmbe.
    char mbe_packet_deinterleaved[49];
    for(int i=0; i<49; i++)
    {
        int j = ysf_dn_decoder_interleave_permutation[i];
        mbe_packet_deinterleaved[i] = mbe_packet_interleaved[j];
    }

    int errs = 0;
    int errs2 = 0;
    char err_str[64];

    short* audio_out_buf = (short*)std::malloc(AUDIO_BUFFER_SIZE);
    mbe_processAmbe2450Data(audio_out_buf, &errs, &errs2, err_str, mbe_packet_deinterleaved, &cur_mp, &prev_mp, &prev_mp_enhanced, uvquality);
    audio_queue->push(audio_out_buf, false);
}

void softmbeSession::decode_imbe_4400(char *input, size_t size)
{
    if(size != 18)
    {
        std::cout << "MBE buffer has " << size << " bytes but should be 18." << std::endl;
        return;
    }

    // First we unpack to bits
    char mbe_packet_interleaved[144];
    for(int i=0; i<144; i++) mbe_packet_interleaved[i] = (input[i/8] >> (7-i%8)) & 0x01;

    // Then we de-interlave the frame (as usual...)
    char mbe_packet_deinterleaved[144];
    for(int i=0; i<144; i++) mbe_packet_deinterleaved[i] = mbe_packet_interleaved[ysf_vw_decoder_interleave_permutation[i]];

    // Then we populate the IMBE frames. Note that for every vector u, libmbe expects us to put
    // In the bits in reverse! Index j is used for this.
    char imbe_fr[8][23] = {0};

    for(int i=  0,j=22; i< 23; i++,j--) imbe_fr[0][j] = mbe_packet_deinterleaved[i]; // u0
    for(int i= 23,j=22; i< 46; i++,j--) imbe_fr[1][j] = mbe_packet_deinterleaved[i]; // u1
    for(int i= 46,j=22; i< 69; i++,j--) imbe_fr[2][j] = mbe_packet_deinterleaved[i]; // u2
    for(int i= 69,j=22; i< 92; i++,j--) imbe_fr[3][j] = mbe_packet_deinterleaved[i]; // u3

    for(int i= 92,j=14; i<107; i++,j--) imbe_fr[4][j] = mbe_packet_deinterleaved[i]; // u4
    for(int i=107,j=14; i<122; i++,j--) imbe_fr[5][j] = mbe_packet_deinterleaved[i]; // u5
    for(int i=122,j=14; i<137; i++,j--) imbe_fr[6][j] = mbe_packet_deinterleaved[i]; // u6

    for(int i=137,j= 6; i<144; i++,j--) imbe_fr[7][j] = mbe_packet_deinterleaved[i]; // u7

    int errs = 0;
    int errs2 = 0;
    char err_str[64];

    short* audio_out_buf = (short*)std::malloc(AUDIO_BUFFER_SIZE);
    char imbe_d[88]  = {0};
    mbe_processImbe7200x4400Frame(audio_out_buf, &errs, &errs2, err_str, imbe_fr, imbe_d, &cur_mp, &prev_mp, &prev_mp_enhanced, uvquality);
    audio_queue->push(audio_out_buf, false);
}

size_t softmbeSession::read(char* output)
{
    short* buf = audio_queue->pop(); // This is a blocking call
    std::memcpy(output, buf, AUDIO_BUFFER_SIZE);
    free(buf);
    return AUDIO_BUFFER_SIZE;
}

void softmbeSession::end() {
    // flush the queue
    while(!audio_queue->empty()) free(audio_queue->pop());
}

CodecServer::proto::FramingHint* softmbeSession::getFraming() {
    unsigned char bits = 0;
    switch(vocoder)
    {
        case VOCODER_NONE:
            return nullptr;
        case VOCODER_AMBE_3600x2450:
        case VOCODER_AMBE_3600x2400:
            bits = 72;
            break;
        case VOCODER_AMBE_2450:
            bits = 49;
            break;
        case VOCODER_IMBE_4400:
            bits = 144;
            break;
        default:
            return nullptr;
    }
    CodecServer::proto::FramingHint* framing;
    framing = new CodecServer::proto::FramingHint();
    framing->set_channelbits(bits);
    framing->set_channelbytes((int)((bits + 7) / 8));
    framing->set_audiosamples(160);
    framing->set_audiobytes(320);
    return framing;
}

void softmbeSession::renegotiate(CodecServer::proto::Settings settings) {
    std::cout << "renegotiating: direction:";
    std::map<std::string, std::string> args(settings.args().begin(), settings.args().end());

    vocoder = VOCODER_NONE; // This will be overwritten below in valid cases
    mbe_initMbeParms(&cur_mp, &prev_mp, &prev_mp_enhanced);

    for (int dir: settings.directions())
    {
        if (dir == Settings_Direction_ENCODE)
        {
            std::cout << " encode (not supported)" <<std::endl;
            return;
        }
        else if (dir == Settings_Direction_DECODE)
        {
            std::cout << " decode; ";
        }
    }

    if (args.find("index") != args.end())
    {
        switch(std::stoi(args["index"]))
        {
            case 33:
                vocoder = VOCODER_AMBE_3600x2450;
                std::cout << "Activating AMBE_3600x2450 decoder.";
                break;
            case 34:
                vocoder = VOCODER_AMBE_2450;
                std::cout << "Activating AMBE_2450 decoder.";
                break;
            default:
                std::cout << "Error: index " << args["index"] << "unsupported.";
                break;
        }
        std::cout << std::endl;
    }
    else if (args.find("ratep") != args.end())
    {
        if(args["ratep"] == "0130:0763:4000:0000:0000:0048")
        {
            // D-STAR
            vocoder = VOCODER_AMBE_3600x2400;
            std::cout << "Activating AMBE_3600x2400 decoder." << std::endl;
        }
        else if(args["ratep"] == "0558:086b:1030:0000:0000:0190")
        {
            // This could be YSF VW but we are not sure
            vocoder = VOCODER_IMBE_4400;
            std::cout << "Activating IMBE_4400 decoder." << std::endl;
        }
        else
        {
            std::cout << "Cannot decode ratep " << args["ratep"] << std::endl;
        }
    } else
    {
        std::cout << "invalid parameters" << std::endl;
    }
}
