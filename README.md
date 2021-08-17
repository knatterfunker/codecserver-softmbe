Introduction
============

[openwebrx](https://www.openwebrx.de/) is a web-based SDR receiver. That software used to be capable of decoding digital voice modes such as DMR, YSF, NXDN and D-STAR. In June 2021, the lead developer [announced](https://groups.io/g/openwebrx/message/3487) the removal of legally dubious support for digital modes based on proprietary audio codecs. According to the announcement, this was done by request of the employer of the lead developer, who enables him to do full-time work on openwebrx (a very generous agreement to say the least) but does not want to be affiliated with a library for decoding the proprietary audio codecs in question.

According to many, such proprietary systems have no place in amateur radio. As demonstrated in this very case, they bar us from experimenting with the technology that we use, but technical experimentation is supposed to be one of the core aspects of amateur radio. We (the authors) tend to agree to all this, yet the technology is out there; removing it from openwebrx is understandable and does make sense, but may also impede some amateur radio enthusiasts' desire for further experimentation. If you use the software from this repository, you are doing so at your own risk and we want you to understand that you may be supporting technology that could entail adverse consequences for amateur radio.

As noted above, support for voice decoding of the aforementioned modes was removed from openwebrx (or rather, from the [digiham](https://github.com/jketterl/digiham/) project entangled with openwebrx) and instead, [codecserver](https://github.com/jketterl/codecserver) was introduced to outsure the decoding of these digital voice frames. codecserver is of modular nature and, at the time of writing, ships with a module that employs a hardware vocoder chip that operates in a black-box manner. With this in place, amateur radio operators who own the proprietary decoding hardware can still listen to digital voice transmissions, but the black-box nature of this solution yet again bars them from any kind of technical experimentation with this technology. It is our understanding that codecserver was designed specifically for easy integration of additional modules, and thus we provide a module that uses the legally dubious [mbelib](https://github.com/szechyjs/mbelib) to decode digital voice in software. This is the same library that was originally used by openwebrx and subsequently removed from the project. This module therefore restores full decoding capabilities of digital voice modes in openwebrx.

Due to fear of legal repercussion, we provide this software anonymously as a written description of how certain voice encoding/decoding algorithms could be implemented.  Executable objects compiled or derived from this package may be covered by one or more patents. Readers are strongly advised to check for any patent restrictions or licencing requirements before compiling or using this source code.

Einführung (Introduction in German)
===================================


[openwebrx](https://www.openwebrx.de/) ist ein webbasierter SDR-Empfänger. Ursprünglich war diese Software in der Lage, digitale Sprachbetriebsarten wie DMR, YSF, NXDN und D-STAR zu decodieren. Im Juni 2021 [kündigte der Hauptentwickler an](https://groups.io/g/openwebrx/message/3487) die Unterstützung für die Decodierung dieser Sprachbetriebsarten zu entfernen, da hierfür eine möglicherweise illegalen Programmbibliothek genutzt wurde. Gemäß dieser Ankündigung unterstütze der Arbeitgeber des Entwicklers dessen Arbeit an openwebrx, möchte aber verständlicherweise nicht mit der allenfalls gesetzeswidrigen Programmbibliothek in Verbindung gebracht werden.

Es ist eine verbreitete Meinung, dass solche proprietären Systeme im Amateurfunk besser nicht Einzug halten sollten. Dieser Fall zeigt gut auf, welche Probleme die Nutzung solcher Technologie nach sich zieht: Es wird eine beachtliche Hürde für das technische Experimentieren eingeführt -- eine Praxis, welche zu den Kernaspekten des Amateurfunks gehört. Diese Argumentation ist gut nachvollziehbar, diese Systeme sind im Amateurfunk aber bereits längst verbreitet und dies kann nicht einfach so rückgängig gemacht werden. Die Unterstützung aus openwebrx zu entfernen ist verständlich und macht Sinn, hindert aber wiederum andere Funkamateure daran, an technischen Experimenten teilzunehmen und ihre technische Neugierde zu befriedigen. Wir bitten Sie deshalb dies alles zu bedenken, falls Sie diese Software nutzen, und wir möchten Sie insbesondere auch darauf hinweisen, dass dem Amateurfunk dadurch Schaden entstehen könnte.

Wie eingangs erwähnt wurde die Unterstützung für die Decodierung der genannten Betriebsarten aus openwebrx (bzw. aus [digiham](https://github.com/jketterl/digiham/), welches mit openwebrx zusammenarbeitet) entfernt. Stattdessen wurde [codecserver](https://github.com/jketterl/codecserver) eingeführt, welches das Decodieren der digitalen Spracheinformation auf eine modulare Art und Weise auslagert. Zum Erscheinungszeitpunkt beinhaltet codecserver ein Modul, welches einen Hardware Vocoder Chip zur Decodierung nutzt. Dieser Chip arbeitet wie eine "black box" -- man weiß nicht, was im Inneren geschieht. Mit dieser Lösung können Funkamateure (sofern sie über die entsprechende proprietäre Hardware verfügen) weiterhin digitale Sprachaussendungen decodieren, aber genau diese "black box" verunmöglicht es ihnen, mit dieser Technologie zu experimentieren. Wir gehen davon aus, dass codecserver ganz bewusst modular gestaltet worden ist, damit weitere Decodiermodule integriert werden können. Wir stellen auf dieser Seite ein Modul zu Verfügung, welches die möglicherweise gesetzeswidrige Bibliothek [mbelib](https://github.com/szechyjs/mbelib) benutzt, um digitale Sprachaussendungen in Software zu decodieren. Es handelt sich dabei um dieselbe Bibliothek, die ursprünglich auch von openwebrx zu diesem Zweck genutzt worden und anschließend entfernt worden ist. Entsprechend stellt diese Software die ursprüngliche Funktionalität von openwebrx wieder her.

Aus Bange vor rechtlichen Konsequenzen publizieren wir dieses Projekt anonym. Der Quelltext beschreibt, wie Aspekte gewisser Sprachbetriebsarten funktionieren könnten. Kompilate aus diesem Quelltext könnten Patentverletzungen darstellen. Die Leserschaft ist deshalb dazu angehalten, diesen Sachverhalt und die notwendigen Maßnahmen abzuklären, bevor sie diese Software nutzen.


Installation
============

First, install openwebrx as usual. We desceribe how to compile and install codecserver-softmbe on Debian Linux and distributions derived from Debian (Raspbian, armbian, Ubuntu, Mint etc.). For other distributions, you can compile and install the project and the libmbe dependency using the standard `cmake` approach.

Install the build dependencies and download, build and install the libmbe dependency:
```
apt install git-core debhelper cmake libprotobuf-dev protobuf-compiler libcodecserver-dev
git clone https://github.com/szechyjs/mbelib.git
cd mbelib
dpkg-buildpackage
cd ..
sudo dpkg -i libmbe1_1.3.0_*.deb libmbe-dev_1.3.0_*.deb
```

Then, download, build and install codecserver-softmbe:
```
git clone https://github.com/knatterfunker/codecserver-softmbe.git
cd codecserver-softmbe
dpkg-buildpackage
cd ..
sudo dpkg -i codecserver-driver-softmbe_0.0.1_*.deb
```

Finally, add the following lines at the end of `/etc/codecserver/codecserver.conf`:
```
[device:softmbe]
driver=softmbe
```

Digital voice decode should now be available in openwebrx after restarting the `openwebrx` and `codecserver` services.
