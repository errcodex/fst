#pragma once

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
}
#include "MediaData/MediaData.h"
#include "MediaData/VideoMediaData.h"

using namespace fst;

class FileHelper
{
public:
	void init(size_t width, size_t height, size_t framerate)
	{
		if (!(avcodecContext = avcodec_alloc_context3(avcodec_find_encoder_by_name("libx264rgb")))) return;
		// if (!(avcodecContext = avcodec_alloc_context3(avcodec_find_encoder(AV_CODEC_ID_H264)))) return;
		if (0 > avformat_alloc_output_context2(&avformatContext, nullptr, nullptr, path)) return;
		if (0 > avio_open(&avformatContext->pb, avformatContext->filename, AVIO_FLAG_WRITE)) return;
		if (!(avstream = avformat_new_stream(avformatContext, nullptr))) return;
		if (!(avframe = av_frame_alloc())) return;
		if (!(avpacket = av_packet_alloc())) return;

		// codec settings
		avcodecContext->time_base = AVRational{ 1, (int)framerate };
		avcodecContext->framerate = AVRational{ (int)framerate, 1 };
		avcodecContext->bit_rate = 1024000;	 // 1M
		avcodecContext->pix_fmt = AV_PIX_FMT_RGB24;
		avcodecContext->width = width;
		avcodecContext->height = height;

		// stream settings
		avstream->time_base = avcodecContext->time_base;
		avcodec_parameters_from_context(avstream->codecpar, avcodecContext);

		if (0 > avcodec_open2(avcodecContext, avcodecContext->codec, nullptr)) return;

		if (0 > avformat_write_header(avformatContext, nullptr)) return;

		// frame settings
		avframe->format = avcodecContext->pix_fmt;
		avframe->width = avcodecContext->width;
		avframe->height = avcodecContext->height;
		avframe->linesize[0] = avframe->width * 3;
		avframe->pts = 0;
	};

	void push(MediaData::Ptr mdPtr)
	{
		if (mdPtr->GetType() != MediaData::MEDIA_TYPE::VIDEO) return;
		auto videoData = std::dynamic_pointer_cast<VideoMediaData>(mdPtr);

		if (videoData->GetVideoType() != VideoMediaData::PIXEL_FORMAT::RGB24) return;

		av_image_fill_arrays(avframe->data, avframe->linesize, videoData->GetData().get(), avcodecContext->pix_fmt, avframe->width, avframe->height, 1);
		encode(avframe, avpacket);

		++avframe->pts;
	};

	void release()
	{
		encode(nullptr, avpacket);
		if (0 != av_write_trailer(avformatContext)) printf("--- ERROR: av_write_trailer");

		avcodec_close(avcodecContext);
		av_free_packet(avpacket);
		av_free(avframe);
		avio_close(avformatContext->pb);
		avformat_free_context(avformatContext);
	}

	void encode(AVFrame* frame, AVPacket* pkt)
	{
		int ret;

		/* send the frame to the encoder */
		if (frame) printf("Send frame %3" PRId64 "\n", frame->pts);

		ret = avcodec_send_frame(avcodecContext, frame);
		if (ret < 0)
		{
			fprintf(stderr, "Error sending a frame for encoding\n");
			exit(1);
		}

		while (ret >= 0)
		{
			ret = avcodec_receive_packet(avcodecContext, pkt);
			if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
				return;
			else if (ret < 0)
			{
				fprintf(stderr, "Error during encoding\n");
				exit(1);
			}

			av_packet_rescale_ts(pkt, avcodecContext->time_base, avstream->time_base);

			av_interleaved_write_frame(avformatContext, pkt);

			av_packet_unref(pkt);
		}
	}

private:
	AVFormatContext* avformatContext = nullptr;
	AVCodecContext* avcodecContext = nullptr;
	AVStream* avstream = nullptr;
	AVFrame* avframe = nullptr;
	AVPacket* avpacket = nullptr;
	const char* path = "test.mp4";
};