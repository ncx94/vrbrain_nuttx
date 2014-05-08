/****************************************************************************
 * apps/examples/json/json_main.c
 *
 * This file is a part of NuttX:
 *
 *   Copyright (C) 2012 Gregory Nutt. All rights reserved.
 *   Ported by: Darcy Gong
 *
 * And derives from the cJSON Project which has an MIT license:
 *
 *   Copyright (c) 2009 Dave Gamble
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <apps/netutils/cJSON.h>

/****************************************************************************
 * Private Types
 ****************************************************************************/

/* Used by some code below as an example datatype. */

struct record
{
  const char *precision;
  double      lat;
  double      lon;
  const char *address;
  const char *city;
  const char *state;
  const char *zip;
  const char *country;
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: doit
 *
 * Description:
 *   Parse text to JSON, then render back to text, and print!
 *
 ****************************************************************************/

static void doit(const char *text)
{
  char *out;
  cJSON *json;
  json = cJSON_Parse(text);
  if (!json)
    {
      printf("Error before: [%s]\n", cJSON_GetErrorPtr());
    }
  else
    {
      out = cJSON_Print(json);
      cJSON_Delete(json);
      printf("%s\n", out);
      free(out);
    }
}

/****************************************************************************
 * Name: dofile
 *
 * Description:
 *   Read a file, parse, render back, etc.
 *
 ****************************************************************************/

#if 0 /* Not used */
static void dofile(char *filename)
{
  FILE *f = fopen(filename, "rb");
  fseek(f, 0, SEEK_END);
  long len = ftell(f);
  fseek(f, 0, SEEK_SET);
  char *data = malloc(len + 1);
  fread(data, 1, len, f);
  fclose(f);
  doit(data);
  free(data);
}
#endif

/****************************************************************************
 * Name: create_objects
 *
 * Description:
 *   Create a bunch of objects as demonstration.
 *
 ****************************************************************************/

static void create_objects(void)
{
  cJSON *root;
  cJSON *fmt;
  cJSON *img;
  cJSON *thm;
  cJSON *fld;
  char *out;
  int i;

  /* Our "days of the week" array */

  static const char *strings[7] =
  {
    "Sunday", "Monday", "Tuesday", "Wednesday",
    "Thursday", "Friday", "Saturday"
  };

  /* Our matrix: */

  static const int numbers[3][3] = { {0, -1, 0}, {1, 0, 0}, {0, 0, 1} };

  /* Our "gallery" item: */

  static const int ids[4] = { 116, 943, 234, 38793 };

  /* Our array of "records": */

  static const struct record fields[2] =
  {
    {"zip", 37.7668, -1.223959e+2, "", "SAN FRANCISCO", "CA", "94107", "US"},
    {"zip", 37.371991, -1.22026e+2, "", "SUNNYVALE", "CA", "94085", "US"}
  };

  /* Here we construct some JSON standards, from the JSON site. */
  /* Our "Video" datatype: */

  root = cJSON_CreateObject();
  cJSON_AddItemToObject(root, "name", cJSON_CreateString("Jack (\"Bee\") Nimble"));
  cJSON_AddItemToObject(root, "format", fmt = cJSON_CreateObject());
  cJSON_AddStringToObject(fmt, "type", "rect");
  cJSON_AddNumberToObject(fmt, "width", 1920);
  cJSON_AddNumberToObject(fmt, "height", 1080);
  cJSON_AddFalseToObject(fmt, "interlace");
  cJSON_AddNumberToObject(fmt, "frame rate", 24);
  out = cJSON_Print(root);
  cJSON_Delete(root);
  printf("%s\n", out);
  free(out);

  /* Print to text, Delete the cJSON, print it, release the string. */

  root = cJSON_CreateStringArray(strings, 7);
  out = cJSON_Print(root);
  cJSON_Delete(root);
  printf("%s\n", out);
  free(out);

  root = cJSON_CreateArray();
  for (i = 0; i < 3; i++)
    {
      cJSON_AddItemToArray(root, cJSON_CreateIntArray(numbers[i], 3));
    }

/*cJSON_ReplaceItemInArray(root,1,cJSON_CreateString("Replacement")); */
  out = cJSON_Print(root);
  cJSON_Delete(root);
  printf("%s\n", out);
  free(out);

  root = cJSON_CreateObject();
  cJSON_AddItemToObject(root, "Image", img = cJSON_CreateObject());
  cJSON_AddNumberToObject(img, "Width", 800);
  cJSON_AddNumberToObject(img, "Height", 600);
  cJSON_AddStringToObject(img, "Title", "View from 15th Floor");
  cJSON_AddItemToObject(img, "Thumbnail", thm = cJSON_CreateObject());
  cJSON_AddStringToObject(thm, "Url", "http:/*www.example.com/image/481989943");
  cJSON_AddNumberToObject(thm, "Height", 125);
  cJSON_AddStringToObject(thm, "Width", "100");
  cJSON_AddItemToObject(img, "IDs", cJSON_CreateIntArray(ids, 4));
  out = cJSON_Print(root);
  cJSON_Delete(root);
  printf("%s\n", out);
  free(out);

  root = cJSON_CreateArray();
  for (i = 0; i < 2; i++)
    {
      cJSON_AddItemToArray(root, fld = cJSON_CreateObject());
      cJSON_AddStringToObject(fld, "precision", fields[i].precision);
      cJSON_AddNumberToObject(fld, "Latitude", fields[i].lat);
      cJSON_AddNumberToObject(fld, "Longitude", fields[i].lon);
      cJSON_AddStringToObject(fld, "Address", fields[i].address);
      cJSON_AddStringToObject(fld, "City", fields[i].city);
      cJSON_AddStringToObject(fld, "State", fields[i].state);
      cJSON_AddStringToObject(fld, "Zip", fields[i].zip);
      cJSON_AddStringToObject(fld, "Country", fields[i].country);
    }

/*cJSON_ReplaceItemInObject(cJSON_GetArrayItem(root,1),"City",cJSON_CreateIntArray(ids,4)); */
  out = cJSON_Print(root);
  cJSON_Delete(root);
  printf("%s\n", out);
  free(out);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/****************************************************************************
 * Name:
 *
 * Description:
 *
 *
 ****************************************************************************/

int json_main(int argc, const char *argv[])
{
  /* a bunch of json: */

  static const char text1[] =
    "{\n"
    "  \"name\": \"Jack (\\\"Bee\\\") Nimble\",\n"
    "  \"format\": {\n"
    "    \"type\":       \"rect\",\n"
    "    \"width\":      1920,\n"
    "    \"height\":     1080,\n"
    "    \"interlace\":  false,\n"
    "    \"frame rate\": 24\n"
    "  }\n"
    "}";

  static const char text2[] =
    "[\"Sunday\", \"Monday\", \"Tuesday\", \"Wednesday\", \"Thursday\", \"Friday\", \"Saturday\"]";

  static const char text3[] =
    "[\n"
    "  [0, -1, 0],\n"
    "  [1, 0, 0],\n"
    "  [0, 0, 1]\n"
    "]\n";

  static const char text4[] =
    "{\n"
    "  \"Image\": {\n"
    "    \"Width\":  800,\n"
    "    \"Height\": 600,\n"
    "    \"Title\":  \"View from 15th Floor\",\n"
    "    \"Thumbnail\": {\n"
    "      \"Url\":    \"http:/*www.example.com/image/481989943\",\n"
    "      \"Height\": 125,\n"
    "      \"Width\":  \"100\"\n"
    "    },\n"
    "  \"IDs\": [116, 943, 234, 38793]\n"
    "  }\n"
    "}";

  static const char text5[] =
    "[\n"
    "  {\n"
    "    \"precision\": \"zip\",\n"
    "    \"Latitude\":  37.7668,\n"
    "    \"Longitude\": -122.3959,\n"
    "    \"Address\":   \"\",\n"
    "    \"City\":      \"SAN FRANCISCO\",\n"
    "    \"State\":     \"CA\",\n"
    "    \"Zip\":       \"94107\",\n"
    "    \"Country\":   \"US\"\n"
    "  },\n"
    "  {\n"
    "    \"precision\": \"zip\",\n"
    "    \"Latitude\":  37.371991,\n"
    "    \"Longitude\": -122.026020,\n"
    "    \"Address\":   \"\",\n"
    "    \"City\":      \"SUNNYVALE\",\n"
    "    \"State\":     \"CA\",\n"
    "    \"Zip\":       \"94085\",\n"
    "    \"Country\":   \"US\"\n"
    "  }\n"
    "]";

  /* Process each json textblock by parsing, then rebuilding: */ doit(text1);

  doit(text2);
  doit(text3);
  doit(text4);
  doit(text5);

  /* Parse standard testfiles: */

#if 0 /* Not yet */
  dofile("../../tests/test1");
  dofile("../../tests/test2");
  dofile("../../tests/test3");
  dofile("../../tests/test4");
  dofile("../../tests/test5");
#endif

  /* Now some samplecode for building objects concisely: */

  create_objects();
  return 0;
}
