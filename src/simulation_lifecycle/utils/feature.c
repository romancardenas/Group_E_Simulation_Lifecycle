
#include "cJSON.h"
#include "stdlib.h"
#include "string.h"
#include "simulation_lifecycle/structures.h"

cJSON * geojson_get_features(cJSON * geo_json) {
    // Whenever you retrieve an array, you have to access child to be able to loop through it with ->next
    return  cJSON_GetObjectItem(geo_json, "features");
}

cJSON * feature_get_properties(cJSON * feature_json) {
    return cJSON_GetObjectItem(feature_json, "properties");
}

cJSON * feature_get_property(cJSON * feature_json, char * property) {
    cJSON * properties = feature_get_properties(feature_json);

    return (properties == NULL) ? NULL : cJSON_GetObjectItem(properties, property);
}

char * feature_get_string_property(cJSON * feature_json, char * property) {
    cJSON * p = feature_get_property(feature_json, property);

    return (p == NULL) ? NULL : cJSON_GetStringValue(p);
}

int * feature_get_int_property(cJSON * feature_json, char * property) {
    cJSON * p = feature_get_property(feature_json, property);

    if (p == NULL) return NULL;

    return &p->valueint;
}

double * feature_get_double_property(cJSON * feature_json, char * property) {
    cJSON * p = feature_get_property(feature_json, property);

    if (p == NULL) return NULL;

    return &p->valuedouble;
}

cJSON * feature_get_geometry(cJSON * feature_json) {
    // Whenever you retrieve an array, you have to access child to be able to loop through it with ->next
    return cJSON_GetObjectItem(feature_json, "geometry");
}

cJSON * feature_get_geometry_coordinates(cJSON * feature_json) {
    cJSON * geometry = feature_get_geometry(feature_json);

    if (geometry == NULL) return NULL;

    return cJSON_GetObjectItem(geometry, "coordinates");
}

char * feature_get_geometry_type(cJSON * feature_json) {
    cJSON * geometry = feature_get_geometry(feature_json);

    if (geometry == NULL) return NULL;

    cJSON * type = cJSON_GetObjectItem(geometry, "type");

    return (type == NULL) ? NULL : cJSON_GetStringValue(type);
}

cJSON * feature_get_first_geometry_in_multi(cJSON * feature_json) {
    char * type = feature_get_geometry_type(feature_json);

    if (strcmp(type, "MultiPoint") != 0 && strcmp(type, "MultiPolygon") != 0 && strcmp(type, "MultiLine") != 0) {
        return NULL;
    }

    cJSON * coordinates = feature_get_geometry_coordinates(feature_json);

    if (coordinates == NULL) return NULL;

    if (cJSON_GetArraySize(coordinates) == 0) return NULL;

    // Not sure about the child part
    return cJSON_GetArrayItem(coordinates, 0);
}

cJSON * feature_get_exterior_ring(cJSON * feature_json) {
    char * type = feature_get_geometry_type(feature_json);
    cJSON * polygon = NULL;

    if (strcmp(type, "MultiPolygon") == 0) {
        polygon = feature_get_first_geometry_in_multi(feature_json);
    }

    else if (strcmp(type, "Polygon") == 0) {
        polygon = feature_json;
    }

    if (polygon == NULL) return NULL;

    // Not sure about the child part
    return cJSON_GetArrayItem(polygon, 0);
}

point_t * json_to_point(cJSON * json) {
    point_t * v = malloc(sizeof(point_t));

    v->lon = cJSON_GetArrayItem(json, 0)->valuedouble;
    v->lat = cJSON_GetArrayItem(json, 1)->valuedouble;

    return v;
}

node_t * json_ring_to_points(cJSON * json) {
    node_t * ring = NULL;

    for (int i = 0; i < cJSON_GetArraySize(json); i++) {
        cJSON * current = cJSON_GetArrayItem(json, i);

        point_t * point = json_to_point(current);

        push_node(&ring, point, sizeof(point_t));
    }

    return ring;
}