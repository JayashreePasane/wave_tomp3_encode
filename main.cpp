#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <iostream>
#include <string>
#include "lame.h"
#include <pthread.h>
#include <thread>
#include <chrono>

using namespace std;

typedef struct Data{
    lame_t lame;
    FILE * wav_file;
    short int * pcm_buffer;
    unsigned char * mp3_buffer;
    unsigned long mp3_buffer_size;
    unsigned long first_sample;
    unsigned long n_samples;
    unsigned long items_read;
    unsigned long mp3_bytes_to_write;
    pthread_mutex_t *mutexForReading;
} Data;

void *encode_chunk(void *arg)
{
    Data * data = (Data *) arg;

    unsigned long offset = 40 + 2 * 2 * data->first_sample;
    pthread_mutex_lock(data->mutexForReading);
    fseek(data->wav_file, offset, SEEK_SET);

    data->items_read = fread(data->pcm_buffer, 2*sizeof(short int) , data->n_samples, data->wav_file);

    cout << "first sample " << data->first_sample << " n_samples "<<  data->n_samples << " items read " << data->items_read << " data address " << data << " mp3 a " << static_cast<void *> (data->mp3_buffer) << endl;
    pthread_mutex_unlock(data->mutexForReading);

    if (data->items_read != 0) 
    {
        data->mp3_bytes_to_write = lame_encode_buffer_interleaved(data->lame, 
                                                                  data->pcm_buffer, 
                                                                  data->items_read,
                                                                  data->mp3_buffer, 
                                                                  data->mp3_buffer_size);
    }
    cout << "finished encoding " << endl;
    return NULL;
}

int main(int argc, char * argv[])
{
    int read,write;

    FILE *wav = fopen("test.wav", "rb");
    FILE *mp3 = fopen("file.mp3", "wb");

    fseek(wav,0,SEEK_END);

	printf("Hello welcome to Encoder\n");
    unsigned long file_size_wav = ftell(wav);
    unsigned long bytes_PCM = file_size_wav - 40;
    unsigned long n_total_samples = bytes_PCM / 4;

    const unsigned long MAX_SAMPLE_NUMBER = pow(2,10);
    const unsigned short NTHREADS = 2;
    const unsigned long MAX_MP3_SIZE = int(MAX_SAMPLE_NUMBER * 1.25 + 7200) + 1;

    short int pcm_buffer[NTHREADS][MAX_SAMPLE_NUMBER * 2]; // 2 channels
    unsigned char mp3_buffer[NTHREADS][MAX_MP3_SIZE]; // according to libmp3lame api

    lame_t lame = lame_init();
    lame_set_in_samplerate(lame, 44100);
    lame_set_VBR(lame, vbr_default);

    // lame_set_brate(lame, 128); // only for CBR mode
    // lame_set_quality(lame, 2); 
    // lame_set_mode(lame, JOINT_STEREO); // 1 joint stereo , 3 mono

    lame_init_params(lame);

    Data data_ptr[NTHREADS];

    unsigned short n_main_loops = n_total_samples / MAX_SAMPLE_NUMBER / NTHREADS + 1;

    cout << "total samples " << n_total_samples << endl;
    cout << "Number of iterations in main loop : " << n_main_loops << endl;

    unsigned long samples_remaining = n_total_samples;
    unsigned long current_sample = 0;

    pthread_t threadID[NTHREADS];
    pthread_mutex_t mutexForReading = PTHREAD_MUTEX_INITIALIZER;

    for (unsigned long i = 0 ; i < n_main_loops; i ++)
    {
        for (unsigned short j = 0; j < NTHREADS; j++ )
        {
            Data data;
            data.lame = lame;
            data.wav_file = wav;
            data.pcm_buffer = pcm_buffer[j];
            data.mp3_buffer = mp3_buffer[j];
            data.first_sample = current_sample;
            data.n_samples = min(MAX_SAMPLE_NUMBER, n_total_samples - current_sample);
            data.mutexForReading = &mutexForReading;

            current_sample += data.n_samples;
            samples_remaining -= data.n_samples;

            data_ptr[j] = data;

            if (data_ptr[j].n_samples > 0)

            {   
                cout << "creating " << i << " " << j << " " << data_ptr[j].first_sample << " " << data_ptr[j].n_samples << endl;
                pthread_create( &threadID[j], 
                           NULL, 
                           encode_chunk, 
                           (void *) (&data_ptr[j]));
            }

        }
        for (unsigned short j = 0; j < NTHREADS; j++)
        {
            if (data_ptr[j].n_samples > 0)
            {   
                pthread_join( threadID[j], NULL); 
            } 
        }

        for (unsigned short j = 0; j< NTHREADS; j++)
            if (data_ptr[j].n_samples > 0)
            {

                fwrite(data_ptr[j].mp3_buffer, data_ptr[j].mp3_bytes_to_write, 1, mp3);
            }
            else
            {
                data_ptr[j].mp3_bytes_to_write = lame_encode_flush(lame, data_ptr[j].mp3_buffer, data_ptr[j].mp3_buffer_size);

            }

    }




    lame_close(lame);
    fclose(mp3);
    fclose(wav);

}