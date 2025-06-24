# M5 Vids

This project is a simple video player experiment I made to test if I could use the M5Stack Cardputer to play videos.

https://github.com/user-attachments/assets/1cad9f04-546c-40a6-82ce-70d8677e0596

https://github.com/user-attachments/assets/01d49514-d8be-49d2-8a9f-848927d2ad81

https://github.com/user-attachments/assets/2f9e45e1-8a41-4943-b442-0deff69500f7


> [!WARNING]
> As I am not a professional MCU/IoT developer, this code may not be the most efficient or optimal solution. It was created as a personal project and may not follow best practices. Use it at your own risk!

This project is essentially a mix of the following projects:

- [ESP32_ScreenShotReceiver](https://github.com/lovyan03/ESP32_ScreenShotReceiver)
  - Thanks to [lovyan03](https://github.com/lovyan03) for the JPEG decoder code (especially the jpgWriteRow implementation)!
- [Play Video With ESP32
@ Instructables](https://www.instructables.com/Play-Video-With-ESP32/) (which seems to be based on the ESP32_ScreenShotReceiver project as well)
  - Thanks to [moononournation](https://github.com/moononournation) for the MJPEG decoder code and all the video transcoding tips!

The project can be built using the Arduino IDE, and it requires no additional libraries other than the standard M5Stack Cardputer libraries.

> [!NOTE]
> The Makefile in the root directory and the compile_flags.txt.in file are just helpers to improve my development experience with the Zed Editor, so they can be ignored.

## How it works

The program plays a video from the SD card using two separated files (one for the video and one for the audio).
The video file must be in the MJPEG format and the audio file must be in the raw PCM u8 format.

> [!TIP]
> There is a Makefile in the `vids` directory that can be used to convert a video file (such as an MP4 file) to the required formats.
> You can use the following command to convert a video file:
> ```bash
> cd vids
> make i=myvideo.mp4
> # which will create myvideo.mjpeg and myvideo.pcm in the `vids/out` directory
> ```

To play the MJPEG+PCM video, just copy the two files to the SD card, insert the SD card into the M5Stack Cardputer, and run the program (the MJPEG file and the PCM file must have the same name, but with the extensions `.mjpeg` and `.pcm`, respectively).

The program supports multiple MJPEG+PCM files, so you can have multiple videos on the SD card.

## Usage

- <kbd>BtnA</kbd> (aka <kbd>BtnG0</kbd>): Pause/Resume the video
- Keyboard <kbd>Left</kbd>/<kbd>Right</kbd> (aka <kbd>,</kbd>/<kbd>/</kbd>): Previous/Next video
- Keyboard <kbd>-</kbd>/<kbd>+</kbd> (aka <kbd>_</kbd>/<kbd>=</kbd>): Volume down/up
