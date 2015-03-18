PKGCONFIG += \
    #cairo-gobject \
    pthreads \
    #libmpg123 \
    gstreamer-riff-1.0 \
    gstreamer-pbutils-1.0 \
    gstreamer-audio-1.0 \
    gstreamer-plugins-bad-1.0  \
    gstreamer-base-1.0 \
    gstreamer-plugins-base-1.0 \
    gstreamer-tag-1.0

QMAKE_LFLAGS += -L$$system($$PKG_CONFIG --variable=pluginsdir gstreamer-1.0)

LIBS += \
    -lgstlibav \
    -lgstapp \
    -lgstavi \
    -lgstcoreelements \
    -lgstd3dvideosink \
    -lgstplayback \
    -lgsttypefindfunctions \
    -lgstdirectsoundsink \
    -lgstmatroska \
    -lgstaudioconvert \
    -lgstaudiomixer \
    -lgstaudioparsers \
    -lgstaudiorate \
    -lgstaudioresample \
    -lgststereo \
    -lgstvolume \
    -lgstwaveformsink \
    -lgstwavparse \
    -lgstautoconvert \
    -lgstautodetect \
    -lgstdirectsoundsrc \
    -lgstwinks \
    -lgstadder \
    -lgstadpcmdec \
    -lgstadpcmenc \
    -lgstaiff \
    -lgstalaw \
    -lgstalpha \
    -lgstalphacolor \
    -lgstasfmux \
    -lgstaudiofxbad \
    -lgstaudiotestsrc \
    -lgstauparse \
    -lgstbayer \
    -lgstlevel \
    -lgstwavenc \
    -lgstwasapi \
    -lgstwinscreencap \
    -lgsty4mdec \
    -lgsty4menc \
    -lgstyadif \
    -lgstudp \
    -lgstvideobox \
    -lgstvideoconvert \
    -lgstvideocrop \
    -lgstvideofilter \
    -lgstvideofiltersbad \
    -lgstvideomixer \
    -lgstvideoparsersbad \
    -lgstvideorate \
    -lgstvideoscale \
    -lgstvideosignal \
    -lgstvideotestsrc \
    -lgstvmnc \
    -lgstdtmf \
    -lgstdvbsuboverlay \
    -lgstdvdspu \
    -lgsteffectv \
    -lgstencodebin \
    -lgstfestival \
    -lgstfieldanalysis \
    -lgstflv \
    -lgstflxdec \
    -lgstfreeverb \
    -lgstfrei0r \
    -lgstgaudieffects \
    -lgstgdp \
    -lgstgeometrictransform \
    -lgstgio \
    -lgstgoom2k1 \
    -lgstgoom \
    -lgsticydemux \
    -lgstid3demux \
    -lgstid3tag \
    -lgstimagefreeze \
    -lgstinter \
    -lgstinterlace \
    -lgstinterleave \
    -lgstisomp4 \
    -lgstivfparse \
    -lgstivtc \
    -lgstjp2kdecimator \
    -lgstjpeg \
    -lgstjpegformat \
    -lgstliveadder \
    -lgstmidi \
    #-lgstmodplug \
    -lgstmpegpsdemux \
    -lgstmpegpsmux \
    -lgstmpegtsdemux \
    -lgstmpegtsmux \
    #-lgstmpg123 \
    -lgstmulaw \
    -lgstmultifile \
    -lgstmultipart \
    -lgstmxf \
    -lgstnavigationtest \
    -lgstpcapparse \
    -lgstpng \
    -lgstpnm \
    -lgstrawparse \
    -lgstremovesilence \
    -lgstreplaygain \
    -lgstrfbsrc \
    -lgstrtpmanager \
    -lgstrtsp \
    -lgstsdpelem \
    -lgstsegmentclip \
    -lgstshapewipe \
    -lgstsmooth \
    -lgstsmpte \
    #-lgstsndfile \
    -lgstspeed \
    -lgstsubenc \
    -lgstsubparse \
    -lgsttcp \
    -lgstcutter \
    #-lgstbz2 \
    #-lgstcairo \
    -lgstcamerabin2 \
    -lgstcoloreffects \
    -lgstcompositor \
    #-lgstogg \
    #-lgstvorbis \
    \
    -lgstrtp-1.0 -lgstbadvideo-1.0 -lgstbadbase-1.0 -lgstcodecparsers-1.0 -lgio-2.0 -lgstnet-1.0 \
    -lgstmpegts-1.0 -lgstsdp-1.0 -lgstrtsp-1.0 -lgstbasecamerabinsrc-1.0 -lgstphotography-1.0 \
    \
    -lavformat \
    -lavcodec \
    -lavutil \
    -lavresample \
    -lavfilter \
    -lswscale \
    -lavdevice \
    -lksuser \
    -lstrmiids \
    -ld3d9 \
    -lsetupapi \
    -lbz2 \
    -ldsound \
    -ldxerr9 \
    -ldnsapi \
    -liphlpapi