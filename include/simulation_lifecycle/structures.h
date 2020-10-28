#ifndef SIMULATION_LIFECYCLE_STRUCTURES_H
#define SIMULATION_LIFECYCLE_STRUCTURES_H

typedef struct workflow {
    // TODO
} workflow_t;

typedef struct feature_set {
    // TODO
} feature_set_t;

typedef struct relation_set {
    // TODO
} relation_set_t;

typedef struct viz_options {
    // TODO
} viz_options_t;

int feature_set_empty(feature_set_t *p_features);  // TODO
int relation_set_empty(relation_set_t *p_relations);  // TODO
int inconsistent_data(feature_set_t *p_features, relation_set_t *p_relations);  // TODO

#endif //SIMULATION_LIFECYCLE_STRUCTURES_H
