# Pixelbrot

![Alt text](public/pixelbrot_image.png?raw=true "Pixelbrot")

A pixel art style mandelbrot visualisation using SDL2

## Pixelating

To pixelate the mandelbrot image a simple sampling procedure is used.  
The image is downsampled to a much smaller resolution and then upsampled back to the original size. In the process details of the image are lost resulting in a pixelated look.

## Setup

```bash
> mkdir bin
> cd bin
> cmake ..
> make
> ./pixelbrot
```

## TODO

- Add more color palettes
- Add screenshot button
