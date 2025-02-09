#include "images.h"

void displayImage (dataset2D * images, int image, int channel)
{
  int current_pixel;
  int coordinate = image * images->row_count * images->column_count * images->channel_count;
  int row_index = images->column_count * images->channel_count;
  for (int row = 0; row < images->row_count; row++)
  {
    for (int column = 0; column < images->column_count; column++)
    {
      current_pixel = images->samples[row * row_index + column * images->channel_count + channel + coordinate];
      if (current_pixel < 10)
      {
        print("  ");
        printUnsignedInteger(current_pixel);
        print(" ");
      }
      else if ((current_pixel >= 10) && (current_pixel < 100))
      {
        print(" ");
        printUnsignedInteger(current_pixel);
        print(" ");       
      }
      else
      {
        printUnsignedInteger(current_pixel);
        print(" ");   
      }
    }
    print("\n");
  }
}