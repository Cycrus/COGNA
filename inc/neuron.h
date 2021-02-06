#ifndef NEURON
#define NEURON

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <stdint.h>
#include <math.h>
#include <time.h>

#define false 0
#define true 1

#define EXCITATORY 1
#define INHIBITORY -1
#define NONDIRECTIONAL 0

#define FUNCTION_SIGMOID 1
#define FUNCTION_LINEAR 2
#define FUNCTION_RELU 3

#define LEARNING_NONE 1
#define LEARNING_HABITUATION 2
#define LEARNING_SENSITIZATION 3
#define LEARNING_HABISENS 4

#define POSITIVE_INFLUENCE 1
#define NEGATIVE_INFLUENCE -1

#define NO_TRANSMITTER -1
#define STD_TRANSMITTER 0

#define DEFAULT_TRANSMITTER_WEIGHT 1.0f

#define DEFAULT_PRESYNAPTIC_POTENTIAL 1.0f

class Neuron;

// Connection structure
struct Connection{
    Neuron* next_neuron;      /* The neuron this connection is connected with */
    Connection* next_connection; /* The connection this connection is connected with */
    Neuron* prev_neuron;      /* The origin neuron of this connection */
    int activation_type;      /* Defines if connection is inhibitory or excitatory */
    int activation_function;  /* Defines what activation function the connection uses */
    int learning_type;        /* Defines the kind of learning the connection is capable of */
    int transmitter_type;     /* Type of neurotransmitter used for transmitting action potential */

    float base_weight;        /* Base weight where learning processes always slowly return to */
    float short_weight;       /* Weight of this connection changing for short term learning. This one is directly used */
    float long_weight;        /* Weight of this connection changing for long term learning */
    float long_learning_weight; /* Factor that controls the learning processes of long_weight */

    float presynaptic_potential;   /* Defines the additional activation by presynaptic connection */
    int64_t last_presynaptic_activated_step;  /* Stores the global network step when this connection was last presynaptic activated */

    int64_t last_activated_step;  /* Stores the global network step when this connection was last activated */
};

class Neuron{
    // TODO Too powerful
	public:
        int _id;
        static int s_max_id;                   /* Last neuron id which has been created. Used for new neuron ids */
        float _threshold;                      /* Threshold when neuron fires */
        int _random_activation;                /* Determines if neuron sometimes fires randomly */
        int _random_chance;                    /* Chance of random activation per tick */
        float _random_activation_value;        /* How strong is the neuron activated when randomly activated */
        float _activation;                     /* Current activation potential level */
        float _temp_activation;                /* A temporary variable to calculate activation of next neurons */
        int64_t _last_activated_step;             /* Network step count, when neuron was last activated */
        int _was_activated;                    /* Indicates if neuron was activated last time or this time */
        int _last_fired_step;                  /* Step when neuron last fired */

        float _activation_backfall_curvature;
        float _activation_backfall_steepness;

        float _short_habituation_curvature;
        float _short_habituation_steepness;
        float _short_sensitization_curvature;
        float _short_sensitization_steepness;
        float _short_dehabituation_curvature;
        float _short_dehabituation_steepness;
        float _short_desensitization_curvature;
        float _short_desensitization_steepness;

        float _long_habituation_curvature;
        float _long_habituation_steepness;
        float _long_sensitization_curvature;
        float _long_sensitization_steepness;
        float _long_dehabituation_curvature;
        float _long_dehabituation_steepness;
        float _long_desensitization_curvature;
        float _long_desensitization_steepness;

        float _presynaptic_potential_curvature;
        float _presynaptic_potential_steepness;
        float _presynaptic_backfall_curvature;
        float _presynaptic_backfall_steepness;

        float _long_learning_weight_reduction_curvature;
        float _long_learning_weight_reduction_steepness;
        float _long_learning_weight_backfall_curvature;
        float _long_learning_weight_backfall_steepness;

        int _influenced_transmitter;            /* Transmitter this neuron should influence if activated */
        int _transmitter_influence_direction;   /* Defines if transmitter is influenced positively or negatively */

        float _habituation_threshold;            /* Threshold until what activation level neuron shall habituate */
        float _sensitization_threshold;          /* Threshold from what activation level neuron neuron shall sensitize */

        std::vector<Connection*> _connections; /* A list of all connections to other neurons */
		std::vector<Neuron*> _previous;        /* A list of all neurons from which this one gets input */

        void set_activation_backfall_curvature(float value);
        void set_activation_backfall_steepness(float value);

        void set_short_habituation_curvature(float value);
        void set_short_habituation_steepness(float value);
        void set_short_sensitization_curvature(float value);
        void set_short_sensitization_steepness(float value);

        void set_short_dehabituation_curvature(float value);
        void set_short_dehabituation_steepness(float value);
        void set_short_desensitization_curvature(float value);
        void set_short_desensitization_steepness(float value);

        void set_long_habituation_curvature(float value);
        void set_long_habituation_steepness(float value);
        void set_long_sensitization_curvature(float value);
        void set_long_sensitization_steepness(float value);

        void set_long_dehabituation_curvature(float value);
        void set_long_dehabituation_steepness(float value);
        void set_long_desensitization_curvature(float value);
        void set_long_desensitization_steepness(float value);

        void set_presynaptic_potential_curvature(float value);
        void set_presynaptic_potential_steepness(float value);
        void set_presynaptic_backfall_curvature(float value);
        void set_presynaptic_backfall_steepness(float value);

        void set_long_learning_weight_reduction_curvature(float value);
        void set_long_learning_weight_reduction_steepness(float value);
        void set_long_learning_weight_backfall_curvature(float value);
        void set_long_learning_weight_backfall_steepness(float value);

        void set_habituation_threshold(float value);
        void set_sensitization_threshold(float value);

        void set_influenced_transmitter(int value);
        void set_transmitter_influence_direction(int value);

        long get_step();
        void set_step(long new_step);

        Neuron(float threshold);
        ~Neuron();

        int check_neuron_connection(Neuron *n);
        int check_synaptic_connection(Connection *n);
        Connection* add_neuron_connection(Neuron *n, float weight,
                                          int con_type=EXCITATORY,
                                          int fun_type=FUNCTION_RELU,
                                          int learn_type=LEARNING_NONE,
                                          int transmitter_type=STD_TRANSMITTER);
        Connection* add_synaptic_connection(Connection *con, float weight,
                                            int con_type=EXCITATORY,
                                            int fun_type=FUNCTION_RELU,
                                            int learn_type=LEARNING_NONE,
                                            int transmitter_type=STD_TRANSMITTER);
        void del_connection(Neuron *n);
        float get_weight(Neuron *n, int type);
        int is_active();
        void set_random_activation(int chance, float activation_value);
};

float choose_activation_function(Connection *con, float input);

#endif