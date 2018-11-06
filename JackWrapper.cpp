//
// Created by emb on 11/5/18.
//

#include "JackWrapper.h"
#include "SoftCut.h"

#include <jack/jack.h>

using namespace softcut;

class JackWrapper::Imp {

    friend class JackWrapper;
private:

    jack_port_t *input_port;
    jack_port_t *output_port;
    jack_client_t *client;

    static int process (jack_nframes_t numFrames, void *data) {
        jack_default_audio_sample_t *in, *out;

        auto * imp = (JackWrapper::Imp*)(data);

        in = (jack_default_audio_sample_t *) jack_port_get_buffer (imp->input_port, numFrames);
        out = (jack_default_audio_sample_t *) jack_port_get_buffer (imp->output_port, numFrames);

        SoftCut& sc = imp->sc;

        sc.processBlockMono(in, out, numFrames);

        return 0;
    }

    static void jack_shutdown(void* data) {
        // nothing to do...
        // but here we would handle forced shutdown from jack server
    }

protected:
    SoftCut sc;

public:
    void setup() {
        const char *client_name = "simple";
        const char *server_name = nullptr;
        jack_options_t options = JackNullOption;
        jack_status_t status;

        /* open a client connection to the JACK server */

        client = jack_client_open (client_name, options, &status, server_name);
        if (client == nullptr) {
            fprintf (stderr, "jack_client_open() failed, "
                    "status = 0x%2.0x\n", status);
            if (status & JackServerFailed) {
                fprintf (stderr, "Unable to connect to JACK server\n");
            }
            exit (1);
        }
        if (status & JackServerStarted) {
            fprintf (stderr, "JACK server started\n");
        }
        if (status & JackNameNotUnique) {
            client_name = jack_get_client_name(client);
            fprintf (stderr, "unique name `%s' assigned\n", client_name);
        }


        printf ("engine sample rate: %" PRIu32 "\n",
                jack_get_sample_rate (client));

        jack_set_process_callback (client, process, this);
        jack_on_shutdown (client, jack_shutdown, this);

        sc.setSampleRate(jack_get_sample_rate(client));

        // create client ports
        input_port = jack_port_register (client, "input",
                                         JACK_DEFAULT_AUDIO_TYPE,
                                         JackPortIsInput, 0);
        output_port = jack_port_register (client, "output",
                                          JACK_DEFAULT_AUDIO_TYPE,
                                          JackPortIsOutput, 0);

        if ((input_port == nullptr) || (output_port == nullptr)) {
            fprintf(stderr, "no more JACK ports available\n");
            exit (1);
        }


    }

    void start() {

        /* Tell the JACK server that we are ready to roll.  Our
          * process() callback will start running now. */

        if (jack_activate (client)) {
            fprintf (stderr, "cannot activate client");
            exit (1);
        }

        /* Connect the ports.  You can't do this before the client is
         * activated, because we can't make connections to clients
         * that aren't running.  Note the confusing (but necessary)
         * orientation of the driver backend ports: playback ports are
         * "input" to the backend, and capture ports are "output" from
         * it.
         */

        const char **ports;
        ports = jack_get_ports (client, nullptr, nullptr,
                                JackPortIsPhysical|JackPortIsOutput);
        if (ports == nullptr) {
            fprintf(stderr, "no physical capture ports\n");
            exit (1);
        }

        if (jack_connect (client, ports[0], jack_port_name (input_port))) {
            fprintf (stderr, "cannot connect input ports\n");
        }

        free (ports);

        ports = jack_get_ports (client, nullptr, nullptr,
                                JackPortIsPhysical|JackPortIsInput);
        if (ports == nullptr) {
            fprintf(stderr, "no physical playback ports\n");
            exit (1);
        }

        if (jack_connect (client, jack_port_name (output_port), ports[0])) {
            fprintf (stderr, "cannot connect output ports\n");
        }

        free (ports);

    }

    void stop() {
        jack_deactivate(client);
    }

    void cleanup() {
        jack_client_close(client);
    }
};

JackWrapper::Imp JackWrapper::imp;

void JackWrapper::setup() {
    JackWrapper::imp.setup();
}

void JackWrapper::cleanup() {
    JackWrapper::imp.cleanup();
}

void JackWrapper::start() {
    JackWrapper::imp.start();
}

void JackWrapper::stop() {
    JackWrapper::imp.stop();
}

SoftCut* JackWrapper::getSoftCut() {
    return &JackWrapper::imp.sc;
}