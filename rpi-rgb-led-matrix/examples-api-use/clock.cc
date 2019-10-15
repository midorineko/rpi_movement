// -*- mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; -*-
// Example of a clock. This is very similar to the text-example,
// except that it shows the time :)
//
// This code is public domain
// (but note, that the led-matrix library this depends on is GPL v2)

#include "led-matrix.h"
#include "graphics.h"

#include <getopt.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

using namespace rgb_matrix;

volatile bool interrupt_received = false;
static void InterruptHandler(int signo) {
  interrupt_received = true;
}

static int usage(const char *progname) {
  fprintf(stderr, "usage: %s [options]\n", progname);
  fprintf(stderr, "Reads text from stdin and displays it. "
          "Empty string: clear screen\n");
  fprintf(stderr, "Options:\n");
  rgb_matrix::PrintMatrixFlags(stderr);
  fprintf(stderr,
          "\t-d <time-format>  : Default '%%H:%%M'. See strftime()\n"
          "\t-f <font-file>    : Use given font.\n"
          "\t-b <brightness>   : Sets brightness percent. Default: 100.\n"
          "\t-x <x-origin>     : X-Origin of displaying text (Default: 0)\n"
          "\t-y <y-origin>     : Y-Origin of displaying text (Default: 0)\n"
          "\t-S <spacing>      : Spacing pixels between letters (Default: 0)\n"
          "\t-C <r,g,b>        : Color. Default 255,255,0\n"
          "\t-B <r,g,b>        : Background-Color. Default 0,0,0\n"
          "\t-O <r,g,b>        : Outline-Color, e.g. to increase contrast.\n"
          );

  return 1;
}

static bool parseColor(Color *c, const char *str) {
  return sscanf(str, "%hhu,%hhu,%hhu", &c->r, &c->g, &c->b) == 3;
}

static bool FullSaturation(const Color &c) {
    return (c.r == 0 || c.r == 255)
        && (c.g == 0 || c.g == 255)
        && (c.b == 0 || c.b == 255);
}

int main(int argc, char *argv[]) {
  RGBMatrix::Options matrix_options;
  rgb_matrix::RuntimeOptions runtime_opt;
  if (!rgb_matrix::ParseOptionsFromFlags(&argc, &argv,
                                         &matrix_options, &runtime_opt)) {
    return usage(argv[0]);
  }

  const char *time_format = "%H:%M";
  Color color(255, 255, 0);
  int redRain[91] = {128, 139, 165, 178, 220, 255, 255, 255, 205, 240, 233, 250, 255, 255, 255, 255, 255, 184, 218, 238, 189, 240, 128, 255, 154, 85, 107, 124, 127, 173, 0, 0, 34, 0, 50, 144, 152, 143, 0, 0, 46, 102, 60, 32, 47, 0, 0, 0, 0, 224, 0, 64, 72, 175, 127, 176, 95, 70, 100, 0, 30, 173, 135, 135, 25, 0, 0, 0, 0, 65, 138, 72, 106, 123, 147, 139, 148, 153, 186, 128, 216, 221, 238, 255, 218, 199, 219, 255, 255, 255, 255};
  int greenRain[91] = {0, 0, 42, 34, 20, 0, 99, 127, 92, 128, 150, 128, 160, 69, 140, 165, 215, 134, 165, 232, 183, 230, 128, 255, 205, 107, 142, 252, 255, 255, 100, 128, 139, 255, 205, 238, 251, 188, 250, 255, 139, 205, 179, 178, 79, 128, 139, 255, 255, 255, 206, 224, 209, 238, 255, 224, 158, 130, 149, 191, 144, 216, 206, 206, 25, 0, 0, 0, 0, 105, 43, 61, 90, 104, 112, 0, 0, 50, 85, 0, 191, 160, 130, 0, 112, 21, 112, 20, 105, 182, 192};
  int blueRain[91] = {0, 0, 42, 34, 60, 0, 71, 80, 92, 128, 122, 114, 122, 0, 0, 0, 0, 11, 32, 170, 107, 140, 0, 0, 50, 47, 35, 0, 0, 47, 0, 0, 34, 0, 50, 144, 152, 143, 154, 127, 87, 170, 113, 170, 79, 128, 139, 255, 255, 255, 209, 208, 204, 238, 212, 230, 160, 180, 237, 255, 255, 230, 235, 250, 112, 128, 139, 205, 255, 225, 226, 139, 205, 238, 219, 139, 211, 204, 211, 128, 216, 221, 238, 255, 214, 133, 147, 147, 180, 193, 203};
  int colorRainInt = 0;
  Color bg_color(0, 0, 0);
  Color outline_color(0,0,0);
  bool with_outline = false;

  const char *bdf_font_file = NULL;
  int x_orig = 0;
  int y_orig = 0;
  int brightness = 100;
  int letter_spacing = 0;

  int opt;
  while ((opt = getopt(argc, argv, "x:y:f:C:B:O:b:S:d:")) != -1) {
    switch (opt) {
    case 'd': time_format = strdup(optarg); break;
    case 'b': brightness = atoi(optarg); break;
    case 'x': x_orig = atoi(optarg); break;
    case 'y': y_orig = atoi(optarg); break;
    case 'f': bdf_font_file = strdup(optarg); break;
    case 'S': letter_spacing = atoi(optarg); break;
    case 'C':
      if (!parseColor(&color, optarg)) {
        fprintf(stderr, "Invalid color spec: %s\n", optarg);
        return usage(argv[0]);
      }
      break;
    case 'B':
      if (!parseColor(&bg_color, optarg)) {
        fprintf(stderr, "Invalid background color spec: %s\n", optarg);
        return usage(argv[0]);
      }
      break;
    case 'O':
      if (!parseColor(&outline_color, optarg)) {
        fprintf(stderr, "Invalid outline color spec: %s\n", optarg);
        return usage(argv[0]);
      }
      with_outline = true;
      break;
    default:
      return usage(argv[0]);
    }
  }

  if (bdf_font_file == NULL) {
    fprintf(stderr, "Need to specify BDF font-file with -f\n");
    return usage(argv[0]);
  }

  /*
   * Load font. This needs to be a filename with a bdf bitmap font.
   */
  rgb_matrix::Font font;
  if (!font.LoadFont(bdf_font_file)) {
    fprintf(stderr, "Couldn't load font '%s'\n", bdf_font_file);
    return 1;
  }
  rgb_matrix::Font *outline_font = NULL;
  if (with_outline) {
      outline_font = font.CreateOutlineFont();
  }

  if (brightness < 1 || brightness > 100) {
    fprintf(stderr, "Brightness is outside usable range.\n");
    return 1;
  }

  RGBMatrix *matrix = rgb_matrix::CreateMatrixFromOptions(matrix_options,
                                                          runtime_opt);
  if (matrix == NULL)
    return 1;

  matrix->SetBrightness(brightness);

  const bool all_extreme_colors = (brightness == 100)
      && FullSaturation(color)
      && FullSaturation(bg_color)
      && FullSaturation(outline_color);
  if (all_extreme_colors)
      matrix->SetPWMBits(1);

  const int x = x_orig;
  int y = y_orig;

  FrameCanvas *offscreen = matrix->CreateFrameCanvas();

  char text_buffer[256];
  struct timespec next_time;
  next_time.tv_sec = time(NULL);
  next_time.tv_nsec = 0;
  struct tm tm;

  signal(SIGTERM, InterruptHandler);
  signal(SIGINT, InterruptHandler);

  while (!interrupt_received) {
      localtime_r(&next_time.tv_sec, &tm);
      strftime(text_buffer, sizeof(text_buffer), time_format, &tm);
      offscreen->Fill(bg_color.r, bg_color.g, bg_color.b);
      if (outline_font) {
          rgb_matrix::DrawText(offscreen, *outline_font,
                               x - 1, y + font.baseline(),
                               outline_color, NULL, text_buffer,
                               letter_spacing - 2);
      }
      rgb_matrix::DrawText(offscreen, font, x, y + font.baseline(),
                           rgb_matrix::Color(redRain[colorRainInt],greenRain[colorRainInt],blueRain[colorRainInt]), NULL, text_buffer,
                           letter_spacing);

      // Wait until we're ready to show it.
      clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &next_time, NULL);

      // Atomic swap with double buffer
      offscreen = matrix->SwapOnVSync(offscreen);

      next_time.tv_sec += 1;
      colorRainInt +=1;
      if(colorRainInt>90){
        colorRainInt = 0;
      }
  }

  // Finished. Shut down the RGB matrix.
  matrix->Clear();
  delete matrix;

  write(STDOUT_FILENO, "\n", 1);  // Create a fresh new line after ^C on screen
  return 0;
}
