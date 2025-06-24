i := input.mp4
vq := 9
url := https://www.youtube.com/watch?v=f6YDKF0LVWw
OUT := out
O_MJPEG := ${OUT}/$(i:.mp4=.mjpeg)
O_PCM := ${OUT}/$(i:.mp4=.pcm)

transcode: $(O_MJPEG) $(O_PCM)

fetch:
	yt-dlp ${url} -f 399+140

${O_MJPEG}: $(i)
	mkdir -p ${OUT}
	ffmpeg -i $< -vf "fps=30,scale=240:-1:flags=lanczos,crop=240:in_h:(in_w-2420)/2:0" -q:v ${vq} -r 30 $@

${O_PCM}: $(i)
	mkdir -p ${OUT}
	ffmpeg -i $< -f u8 -acodec pcm_u8 -ar 44100 -ac 1 $@

.PHONY: transcode fetch
