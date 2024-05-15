#pragma once

#include <memory>

namespace ff {
	class FFAVPacketImpl;
	class FFAVFormatContextImpl;
    class FFAVCodecContextImpl;
    class FFAVFrameImpl;
    class FFAVStreamImpl;
    class FFAVChannelLayoutImpl;
    class FFAVCodecParametersImpl;
    class FFAVSwsContextImpl;

	typedef std::shared_ptr<FFAVPacketImpl>		   FFAVPacketImplPtr;
	typedef std::shared_ptr<FFAVFormatContextImpl> FFAVFormatContextImplPtr;
    typedef std::shared_ptr<FFAVCodecContextImpl>  FFAVCodecContextImplPtr;
    typedef std::shared_ptr<FFAVFrameImpl>         FFAVFrameImplPtr;
    typedef std::shared_ptr<FFAVStreamImpl>        FFAVStreamImplPtr;
    typedef std::shared_ptr<FFAVChannelLayoutImpl> FFAVChannelLayoutImplPtr;
    typedef std::shared_ptr<FFAVCodecParametersImpl> FFAVCodecParametersImplPtr;
    typedef std::shared_ptr<FFAVSwsContextImpl>    FFAVSwsContextImplPtr;
};