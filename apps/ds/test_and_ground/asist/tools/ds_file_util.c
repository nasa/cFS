//
// ds_file_util.c : Format a binary DS or IM file
//

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define FOUR_BYTE_FIELD       4
#define ONE_BYTE_FIELD        1
#define HDR_BYTES             140
#define FILE_NAME_BYTES       64
#define TLM_PRI_HDR_BYTES     6
#define TLM_SEC_HDR_BYTES     6
#define MAX_PACKET_BYTES      100000
#define PRINT_COLUMNS         16
#define TRUE                  1
#define FALSE                 0
#define MAX_FILE_NAMES        1000

int main(int argc, char **argv)
{
   FILE *infile;
   FILE *outfile;
   FILE *compare_file;
   int i;
   int j;
   int n;
   int print_rows;
   int actual_data_length;
   size_t number_of_files = 0;;
   int file_num;
   char headers_only_flag[3];
   unsigned char file_header[HDR_BYTES];
   unsigned char packet_data[MAX_PACKET_BYTES];
   unsigned char infile_names[MAX_FILE_NAMES][FILE_NAME_BYTES];
   unsigned char outfile_name[FILE_NAME_BYTES];
   unsigned char filename[FILE_NAME_BYTES];
   int num_pkt_types = 0;
   int end_of_data = FALSE;
   int total_bytes = 0;
   int verbose = FALSE;
   int compare = FALSE;
   int write_header = FALSE;

   struct {
      int version;
      int type;
      int sec_hdr_flg;
      int app_id;
      int seg_flags;
      int seq_count;
      int pkt_len;

   } ccsds_pri_hdr;

   struct
   {
      int reserved;
      int seconds;
      int subseconds;
   } ccsds_sec_hdr;

   union {
      unsigned char byteStream[4];
      unsigned int value;
   } fourByte;

   union {
      unsigned char byteStream[2];
      short value;
   } twoByte;

   if (sizeof(int) != FOUR_BYTE_FIELD)
   {
      fprintf(outfile,"Error: integer not 4 bytes.\n");
      exit(1);
   }

   if (argc < 2)
   {
      printf("\n");
      printf(" Usage: fsw_futil <input_file1, input_file2...> [OPTIONS]\n");
      printf("\n");
      printf(" Description:\n");
      printf("     ds_file_util will process the specified input_file(s)\n");
      printf("     wildcards accepted), which will be binary files stored by \n");
      printf("     cFS DS application, and create an ascii breakdown as an \n");
      printf("     output file with '.txt' appended. Also, a raw ascii output\n");
      printf("     file will be created with only packet data (no headers or \n");
      printf("     other text) and will have '_raw.txt' appended.\n\n");
      printf(" Example: >ds_file_util Crater*.sci -h -v\n\n");
      printf(" OPTIONS:\n");
      printf("\n");
      printf("  -h   Ouput packet headers only\n");
      printf("  -v   Verbose output to screen (debugging)\n\n");
   }

   /*
   ** Process command line arguments
   */
   for (i = 1; i < argc; i++)
   {
      if (argv[i][0] != '-')
      {
         strcpy(infile_names[i-1],argv[i]);
         number_of_files++;
      }
      else if (argv[i][1] == 'h')
      {
         headers_only_flag[0] = 'y';
      }
      else if (argv[i][1] == 'v')
      {
         verbose = TRUE;
      }
   }

   if (verbose == TRUE)
   {
      printf("\nNumber of files: %d\n",number_of_files);
   }

   for (file_num = 0; file_num < number_of_files; file_num++)
   {
      if (verbose == TRUE)
      {
         printf("\n File %d: %s \n", file_num, infile_names[file_num]);
      }

      if (! (infile = fopen((const char *)infile_names[file_num],"rb")))
      {
         printf("Cannot open input file: %s\n",infile_names[file_num]);
         exit(1);
      }

      strcpy(filename,infile_names[file_num]);

      if (! (outfile = fopen((const char *)strcat(filename,".txt"),"w")))
      {
         printf("Cannot create output file.\n");
         exit(1);
      }

      strcpy(filename,infile_names[file_num]);

      if (! (compare_file = fopen((const char *)strcat(filename,"_raw.txt"),"w")))
      {
         printf("Cannot create output file.\n");
         exit(1);
      }

      num_pkt_types   = 0;
      end_of_data     = FALSE;
      write_header    = TRUE;
      total_bytes     = 0;

      /*
      ** Read the cFE and DS file headers
      */
      n = fread(file_header, ONE_BYTE_FIELD, HDR_BYTES, infile);

      if (n != HDR_BYTES)
      {
         fprintf(outfile,"Error reading file header.\n");
         exit(1);
      }

      /*
      ** Write header to the *_raw.txt file -- WFM
      */
      if (write_header == TRUE)
      {
        for (i = 0; i < HDR_BYTES; i+=2)
	{
          fprintf(compare_file,"%02X%02X ",file_header[i],file_header[i+1]);
        }
      }

      fprintf(outfile,"FILE HEADER: (hex) - (value)\n");
      fprintf(outfile,"----------------------------\n");

      /* Extract the Content Type */
      unsigned char contentType[5];
      strncpy(&contentType[0],&file_header[0],4);
      contentType[4] = '\0';

      fprintf(outfile,"Content Type: %02X%02X%02X%02X - %s\n",file_header[0],file_header[1],file_header[2],file_header[3],&contentType);

      /* Extract the subtype */
      unsigned char ByteVal4[4];
      int subType=0;
      strncpy(&ByteVal4[0],&file_header[6],2);
      subType = atoi(&ByteVal4[0]);

      fprintf(outfile,"SubType:      %02X%02X - %d\n",file_header[6],file_header[7],subType);

      /* Extract the Primary Header Length */
      fourByte.byteStream[0] = file_header[11];
      fourByte.byteStream[1] = file_header[10];
      fourByte.byteStream[2] = file_header[9];
      fourByte.byteStream[3] = file_header[8];

      fprintf(outfile,"Primary Hdr Length: %02X%02X%02X%02X - %d\n",file_header[8],file_header[9],file_header[10],file_header[11],fourByte.value);

      /* Extract the Spacecraft ID */
      fourByte.byteStream[0] = file_header[15];
      fourByte.byteStream[1] = file_header[14];
      fourByte.byteStream[2] = file_header[13];
      fourByte.byteStream[3] = file_header[12];

      fprintf(outfile,"Spacecraft ID:  %02X%02X%02X%02X - %d\n",file_header[12],file_header[13],file_header[14],file_header[15],fourByte.value);

      /* Extract the Processor ID */
      fourByte.byteStream[0] = file_header[19];
      fourByte.byteStream[1] = file_header[18];
      fourByte.byteStream[2] = file_header[17];
      fourByte.byteStream[3] = file_header[16];

      fprintf(outfile,"Processor ID:   %02X%02X%02X%02X - %d\n",file_header[16],file_header[17],file_header[18],file_header[19],fourByte.value);

      /* Extract the Application ID */
      fourByte.byteStream[0] = file_header[23];
      fourByte.byteStream[1] = file_header[22];
      fourByte.byteStream[2] = file_header[21];
      fourByte.byteStream[3] = file_header[20];

      fprintf(outfile,"Application ID: %02X%02X%02X%02X - %d\n",file_header[20],file_header[21],file_header[22],file_header[23],fourByte.value);

      /* Extract the Create Time seconds */
      fourByte.byteStream[0] = file_header[27];
      fourByte.byteStream[1] = file_header[26];
      fourByte.byteStream[2] = file_header[25];
      fourByte.byteStream[3] = file_header[24];

      fprintf(outfile,"Create Time (secs): %02X%02X%02X%02X - %u\n",file_header[24],file_header[25],file_header[26],file_header[27],fourByte.value);

      /* Extract the Create Time sub-seconds */
      fourByte.byteStream[0] = file_header[31];
      fourByte.byteStream[1] = file_header[30];
      fourByte.byteStream[2] = file_header[29];
      fourByte.byteStream[3] = file_header[28];

      fprintf(outfile,"Create Time (subs): %02X%02X%02X%02X - %u\n",file_header[28],file_header[29],file_header[30],file_header[31],fourByte.value);

      unsigned char description[32];
      strncpy(&description[0],&file_header[32],32);
      fprintf(outfile,"Description: %s\n",description);

      /* Extract the Create Time seconds */
      fourByte.byteStream[0] = file_header[67];
      fourByte.byteStream[1] = file_header[66];
      fourByte.byteStream[2] = file_header[65];
      fourByte.byteStream[3] = file_header[64];

      /* Extract the DS Application File Header */
      fprintf(outfile,"Close Time (secs): %02X%02X%02X%02X - %u\n",file_header[64],file_header[65],file_header[66],file_header[67],fourByte.value);

      /* Extract the Create Time sub-seconds */
      fourByte.byteStream[0] = file_header[71];
      fourByte.byteStream[1] = file_header[70];
      fourByte.byteStream[2] = file_header[69];
      fourByte.byteStream[3] = file_header[68];

      fprintf(outfile,"Close Time (subs): %02X%02X%02X%02X - %u\n",file_header[68],file_header[69],file_header[70],file_header[71],fourByte.value);

      /* Extract the File Table Index */
      twoByte.byteStream[0] = file_header[73];
      twoByte.byteStream[1] = file_header[72];

      fprintf(outfile,"File Table Index:  %02X%02X - %d\n",file_header[72],file_header[73],twoByte.value);

      /* Extract the File Name Type */
      twoByte.byteStream[0] = file_header[75];
      twoByte.byteStream[1] = file_header[74];

      fprintf(outfile,"File Name Type:    %02X%02X - %d\n",file_header[74],file_header[75],twoByte.value);

      unsigned char dsFileName[64];
      strncpy(&dsFileName[0],&file_header[76],64);
      fprintf(outfile,"File Name: %s\n",dsFileName);

      actual_data_length = MAX_PACKET_BYTES;

      /*
      ** Header processing finished, now read in the packet data
      */
      n = fread(packet_data, ONE_BYTE_FIELD, actual_data_length, infile);

      if (n != actual_data_length)
      {
         end_of_data = TRUE;
         actual_data_length = n;
      }

      total_bytes += n;

      /*
      ** Write the raw data to the *_raw.txt file
      */
      for (i = 0; i < actual_data_length; i+=2)
      {
         fprintf(compare_file,"%02X%02X ",packet_data[i],packet_data[i+1]);
      }
      fprintf(compare_file,"\n");

      /*
      ** Write the formatted data fields to the output files
      */
      if ((headers_only_flag[0] != 'y') && (headers_only_flag[0] != 'Y'))
      {
         fprintf(outfile,"\nPACKET DATA: (hex)\n------------------\n\n");

         print_rows = actual_data_length/PRINT_COLUMNS;

         for (i = 0; i < print_rows; i++)
         {
            fprintf(outfile,"%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X\n",
               packet_data[i*PRINT_COLUMNS],
               packet_data[i*PRINT_COLUMNS+1],
               packet_data[i*PRINT_COLUMNS+2],
               packet_data[i*PRINT_COLUMNS+3],
               packet_data[i*PRINT_COLUMNS+4],
               packet_data[i*PRINT_COLUMNS+5],
               packet_data[i*PRINT_COLUMNS+6],
               packet_data[i*PRINT_COLUMNS+7],
               packet_data[i*PRINT_COLUMNS+8],
               packet_data[i*PRINT_COLUMNS+9],
               packet_data[i*PRINT_COLUMNS+10],
               packet_data[i*PRINT_COLUMNS+11],
               packet_data[i*PRINT_COLUMNS+12],
               packet_data[i*PRINT_COLUMNS+13],
               packet_data[i*PRINT_COLUMNS+14],
               packet_data[i*PRINT_COLUMNS+15]);
         }

         for (i = 0; i < (actual_data_length % PRINT_COLUMNS); i++)
         {
            fprintf(outfile,"%02X ",packet_data[print_rows*PRINT_COLUMNS+i]);
         }

         fprintf(outfile,"\n\n");
      }

     /* Print summary */
     fprintf(outfile,"\n\nFile Summary:\n\n");

     /* Add file header bytes to byte count */
     total_bytes += HDR_BYTES;
     fprintf(outfile,"Total Bytes: %d\n", total_bytes);
   }

   return 0;
}

