#pragma once

#include "session.hpp"
#include "softmbedevice.hpp"
#include "blockingqueue.hpp"
#include "proto/framing.pb.h"
#include "proto/request.pb.h"
#include <unistd.h>

extern "C" {
    #include <mbelib.h>
}

namespace softmbe
{

    enum vocoder_t
    {
        VOCODER_NONE, VOCODER_IMBE_4400, VOCODER_AMBE_2450,
        VOCODER_AMBE_3600x2400, VOCODER_AMBE_3600x2450
    };

    class softmbeSession: public CodecServer::Session
    {
        public:
            softmbeSession(int q);
            void encode(char* input, size_t size) override;
            void decode(char* input, size_t size) override;
            size_t read(char* output) override;
            void end() override;
            FramingHint* getFraming() override;
            void renegotiate(Settings settings) override;
        private:
            // The queue holds arrays of 160 shorts of decoded audio
            BlockingQueue<short> *audio_queue;

            vocoder_t vocoder;

            mbe_parms cur_mp;
            mbe_parms prev_mp;
            mbe_parms prev_mp_enhanced;
            int uvquality; // for mbe_synthesizeSpeechf, default is 3

            void decode_ambe_3600x2450(char* input, size_t size);
            void decode_ambe_3600x2400(char* input, size_t size);
            void decode_ambe_2450(char* input, size_t size);
            void decode_imbe_4400(char* input, size_t size);
    };

}
