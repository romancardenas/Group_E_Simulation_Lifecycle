#include <unity.h>
#include "simulation_lifecycle/error.h"
#include "simulation_lifecycle/visualization.h"
#include "simulation_lifecycle/utils/file.h"

void setUp(void) {
    remove("../test/data/visualization/output/visualization/provinces.geojson");
    remove("../test/data/visualization/output/visualization/visualization.json");
    remove("../test/data/visualization/output/visualization/structure.json");
    remove("../test/data/visualization/output/visualization/messages.log");
    remove("../test/data/visualization/output_missing_conversion/visualization/provinces.json");
    remove("../test/data/visualization/output_missing_conversion/visualization/visualization.json");
    remove("../test/data/visualization/output_missing_conversion/visualization/structure.json");
    remove("../test/data/visualization/output_missing_conversion/visualization/messages.log");
}

void tearDown(void) {
    /*
    remove("../test/data/visualization/output/visualization/provinces.geojson");
    remove("../test/data/visualization/output/visualization/visualization.json");
    remove("../test/data/visualization/output/visualization/structure.json");
    remove("../test/data/visualization/output/visualization/messages.log");
    remove("../test/data/visualization/output_missing_conversion/visualization/provinces.geojson");
    remove("../test/data/visualization/output_missing_conversion/visualization/visualization.json");
    remove("../test/data/visualization/output_missing_conversion/visualization/structure.json");
    remove("../test/data/visualization/output_missing_conversion/visualization/messages.log");
     */
}

void test_validate_visualization(void) {
    char * json = "{\"basemap\":\"openstreetmap\",\"view\":{\"center\":[-89,57],\"zoom\":5},\"layers\":[{\"id\":\"provinces\",\"type\":\"polygon\",\"file\":\"../test/data/visualization/input/provinces.geojson\",\"label\":\"Provinces\",\"style\":\"default\",\"model\":\"pandemic\",\"join\":\"pruid\"}],\"simulation\":[{\"name\":\"recovered\",\"layer\":\"provinces\",\"fill\":{\"type\":\"quantile\",\"property\":\"recovered\",\"colors\":[\"rgba(118,42,131,0.85)\",\"rgba(153,112,171,0.85)\",\"rgba(194,165,207,0.85)\",\"rgba(231,212,232,0.85)\",\"rgba(247,247,247,0.85)\",\"rgba(217,240,211,0.85)\",\"rgba(166,219,160,0.85)\",\"rgba(90,174,97,0.85)\",\"rgba(27,120,55,0.85)\"]},\"stroke\":{\"type\":\"static\",\"width\":1,\"color\":\"rgba(0,0,0,1)\"}},{\"name\":\"population\",\"layer\":\"provinces\",\"fill\":{\"type\":\"equivalent\",\"property\":\"population\",\"colors\":[\"rgba(215,48,39,0.85)\",\"rgba(244,109,67,0.85)\",\"rgba(253,174,97,0.85)\",\"rgba(254,224,144,0.85)\",\"rgba(255,255,191,0.85)\",\"rgba(224,243,248,0.85)\",\"rgba(171,217,233,0.85)\",\"rgba(116,173,209,0.85)\",\"rgba(69,117,180,0.85)\"]},\"stroke\":{\"type\":\"static\",\"width\":1,\"color\":\"rgba(0,0,0,1)\"}},{\"name\":\"susceptible\",\"layer\":\"provinces\",\"fill\":{\"type\":\"quantile\",\"property\":\"susceptible\",\"colors\":[\"rgba(247,252,240,0.85)\",\"rgba(224,243,219,0.85)\",\"rgba(204,235,197,0.85)\",\"rgba(168,221,181,0.85)\",\"rgba(123,204,196,0.85)\",\"rgba(78,179,211,0.85)\",\"rgba(43,140,190,0.85)\",\"rgba(8,104,172,0.85)\",\"rgba(8,64,129,0.85)\"]},\"stroke\":{\"type\":\"static\",\"width\":1,\"color\":\"rgba(0,0,0,1)\"}},{\"name\":\"infected\",\"layer\":\"provinces\",\"fill\":{\"type\":\"quantile\",\"property\":\"infected\",\"colors\":[\"rgba(140,81,10,0.85)\",\"rgba(191,129,45,0.85)\",\"rgba(223,194,125,0.85)\",\"rgba(246,232,195,0.85)\",\"rgba(245,245,245,0.85)\",\"rgba(199,234,229,0.85)\",\"rgba(128,205,193,0.85)\",\"rgba(53,151,143,0.85)\",\"rgba(1,102,94,0.85)\"]},\"stroke\":{\"type\":\"static\",\"width\":1,\"color\":\"rgba(0,0,0,1)\"}}]}";
    cJSON * viz_data = cJSON_Parse(json);

    int res = validate_visualization(viz_data);
    TEST_ASSERT_EQUAL(SUCCESS, res);

    cJSON_DeleteItemFromObject(viz_data, "basemap");
    res = validate_visualization(viz_data);
    TEST_ASSERT_EQUAL(VIZ_BAD_BASEMAP, res);
    cJSON_AddStringToObject(viz_data, "basemap", "openstreetmap");
}

void test_validate_view(void) {
    char * json = "{\"basemap\":\"openstreetmap\",\"view\":{\"center\":[-89,57],\"zoom\":5},\"layers\":[{\"id\":\"provinces\",\"type\":\"polygon\",\"file\":\"../test/data/visualization/input/provinces.geojson\",\"label\":\"Provinces\",\"style\":\"default\",\"model\":\"pandemic\",\"join\":\"pruid\"}],\"simulation\":[{\"name\":\"recovered\",\"layer\":\"provinces\",\"fill\":{\"type\":\"quantile\",\"property\":\"recovered\",\"colors\":[\"rgba(118,42,131,0.85)\",\"rgba(153,112,171,0.85)\",\"rgba(194,165,207,0.85)\",\"rgba(231,212,232,0.85)\",\"rgba(247,247,247,0.85)\",\"rgba(217,240,211,0.85)\",\"rgba(166,219,160,0.85)\",\"rgba(90,174,97,0.85)\",\"rgba(27,120,55,0.85)\"]},\"stroke\":{\"type\":\"static\",\"width\":1,\"color\":\"rgba(0,0,0,1)\"}},{\"name\":\"population\",\"layer\":\"provinces\",\"fill\":{\"type\":\"equivalent\",\"property\":\"population\",\"colors\":[\"rgba(215,48,39,0.85)\",\"rgba(244,109,67,0.85)\",\"rgba(253,174,97,0.85)\",\"rgba(254,224,144,0.85)\",\"rgba(255,255,191,0.85)\",\"rgba(224,243,248,0.85)\",\"rgba(171,217,233,0.85)\",\"rgba(116,173,209,0.85)\",\"rgba(69,117,180,0.85)\"]},\"stroke\":{\"type\":\"static\",\"width\":1,\"color\":\"rgba(0,0,0,1)\"}},{\"name\":\"susceptible\",\"layer\":\"provinces\",\"fill\":{\"type\":\"quantile\",\"property\":\"susceptible\",\"colors\":[\"rgba(247,252,240,0.85)\",\"rgba(224,243,219,0.85)\",\"rgba(204,235,197,0.85)\",\"rgba(168,221,181,0.85)\",\"rgba(123,204,196,0.85)\",\"rgba(78,179,211,0.85)\",\"rgba(43,140,190,0.85)\",\"rgba(8,104,172,0.85)\",\"rgba(8,64,129,0.85)\"]},\"stroke\":{\"type\":\"static\",\"width\":1,\"color\":\"rgba(0,0,0,1)\"}},{\"name\":\"infected\",\"layer\":\"provinces\",\"fill\":{\"type\":\"quantile\",\"property\":\"infected\",\"colors\":[\"rgba(140,81,10,0.85)\",\"rgba(191,129,45,0.85)\",\"rgba(223,194,125,0.85)\",\"rgba(246,232,195,0.85)\",\"rgba(245,245,245,0.85)\",\"rgba(199,234,229,0.85)\",\"rgba(128,205,193,0.85)\",\"rgba(53,151,143,0.85)\",\"rgba(1,102,94,0.85)\"]},\"stroke\":{\"type\":\"static\",\"width\":1,\"color\":\"rgba(0,0,0,1)\"}}]}";
    cJSON * viz_data = cJSON_Parse(json);

    cJSON * view = cJSON_GetObjectItem(viz_data, "view");

    cJSON_DeleteItemFromObject(view, "center");
    int res = validate_visualization(viz_data);
    TEST_ASSERT_EQUAL(VIZ_LAYER_VIEW_BAD_CENTER, res);
    cJSON_AddItemReferenceToObject(view, "center", cJSON_Parse("[-89, 57]"));

    cJSON_DeleteItemFromObject(view, "zoom");
    res = validate_visualization(viz_data);
    TEST_ASSERT_EQUAL(VIZ_LAYER_VIEW_BAD_ZOOM, res);
    cJSON_AddNumberToObject(view, "zoom", 5);
}

void test_validate_layers(void) {
    char * json = "{\"basemap\":\"openstreetmap\",\"view\":{\"center\":[-89,57],\"zoom\":5},\"layers\":[{\"id\":\"provinces\",\"type\":\"polygon\",\"file\":\"../test/data/visualization/input/provinces.geojson\",\"label\":\"Provinces\",\"style\":\"default\",\"model\":\"pandemic\",\"join\":\"pruid\"}],\"simulation\":[{\"name\":\"recovered\",\"layer\":\"provinces\",\"fill\":{\"type\":\"quantile\",\"property\":\"recovered\",\"colors\":[\"rgba(118,42,131,0.85)\",\"rgba(153,112,171,0.85)\",\"rgba(194,165,207,0.85)\",\"rgba(231,212,232,0.85)\",\"rgba(247,247,247,0.85)\",\"rgba(217,240,211,0.85)\",\"rgba(166,219,160,0.85)\",\"rgba(90,174,97,0.85)\",\"rgba(27,120,55,0.85)\"]},\"stroke\":{\"type\":\"static\",\"width\":1,\"color\":\"rgba(0,0,0,1)\"}},{\"name\":\"population\",\"layer\":\"provinces\",\"fill\":{\"type\":\"equivalent\",\"property\":\"population\",\"colors\":[\"rgba(215,48,39,0.85)\",\"rgba(244,109,67,0.85)\",\"rgba(253,174,97,0.85)\",\"rgba(254,224,144,0.85)\",\"rgba(255,255,191,0.85)\",\"rgba(224,243,248,0.85)\",\"rgba(171,217,233,0.85)\",\"rgba(116,173,209,0.85)\",\"rgba(69,117,180,0.85)\"]},\"stroke\":{\"type\":\"static\",\"width\":1,\"color\":\"rgba(0,0,0,1)\"}},{\"name\":\"susceptible\",\"layer\":\"provinces\",\"fill\":{\"type\":\"quantile\",\"property\":\"susceptible\",\"colors\":[\"rgba(247,252,240,0.85)\",\"rgba(224,243,219,0.85)\",\"rgba(204,235,197,0.85)\",\"rgba(168,221,181,0.85)\",\"rgba(123,204,196,0.85)\",\"rgba(78,179,211,0.85)\",\"rgba(43,140,190,0.85)\",\"rgba(8,104,172,0.85)\",\"rgba(8,64,129,0.85)\"]},\"stroke\":{\"type\":\"static\",\"width\":1,\"color\":\"rgba(0,0,0,1)\"}},{\"name\":\"infected\",\"layer\":\"provinces\",\"fill\":{\"type\":\"quantile\",\"property\":\"infected\",\"colors\":[\"rgba(140,81,10,0.85)\",\"rgba(191,129,45,0.85)\",\"rgba(223,194,125,0.85)\",\"rgba(246,232,195,0.85)\",\"rgba(245,245,245,0.85)\",\"rgba(199,234,229,0.85)\",\"rgba(128,205,193,0.85)\",\"rgba(53,151,143,0.85)\",\"rgba(1,102,94,0.85)\"]},\"stroke\":{\"type\":\"static\",\"width\":1,\"color\":\"rgba(0,0,0,1)\"}}]}";
    cJSON * viz_data = cJSON_Parse(json);

    cJSON * layer = cJSON_GetArrayItem(cJSON_GetObjectItem(viz_data, "layers"), 0);

    cJSON_SetValuestring(cJSON_GetObjectItem(layer, "type"), "non-existent");
    int res = validate_visualization(viz_data);
    TEST_ASSERT_EQUAL(VIZ_LAYER_BAD_TYPE, res);
    cJSON_SetValuestring(cJSON_GetObjectItem(layer, "type"), "polygon");

    cJSON_SetValuestring(cJSON_GetObjectItem(layer, "file"), "non-existent");
    res = validate_visualization(viz_data);
    TEST_ASSERT_EQUAL(VIZ_LAYER_BAD_FILE, res);
    cJSON_SetValuestring(cJSON_GetObjectItem(layer, "file"), "../test/data/visualization/input/provinces.geojson");

    cJSON_DeleteItemFromObject(layer, "label");
    res = validate_visualization(viz_data);
    TEST_ASSERT_EQUAL(VIZ_LAYER_BAD_LABEL, res);
    cJSON_AddStringToObject(layer, "label", "Provinces");

    cJSON_DeleteItemFromObject(layer, "style");
    res = validate_visualization(viz_data);
    TEST_ASSERT_EQUAL(VIZ_LAYER_BAD_STYLE, res);
    cJSON_AddStringToObject(layer, "style", "default");

    cJSON_DeleteItemFromObject(layer, "model");
    res = validate_visualization(viz_data);
    TEST_ASSERT_EQUAL(SUCCESS, res);

    cJSON_AddNumberToObject(layer, "model", 1);
    res = validate_visualization(viz_data);
    TEST_ASSERT_EQUAL(VIZ_LAYER_BAD_MODEL, res);

    cJSON_DeleteItemFromObject(layer, "model");
    cJSON_AddStringToObject(layer, "model", "pandemic");

    cJSON_DeleteItemFromObject(layer, "join");
    res = validate_visualization(viz_data);
    TEST_ASSERT_EQUAL(SUCCESS, res);

    cJSON_AddNumberToObject(layer, "join", 1);
    res = validate_visualization(viz_data);
    TEST_ASSERT_EQUAL(VIZ_LAYER_BAD_JOIN, res);

    cJSON_DeleteItemFromObject(layer, "join");
    cJSON_AddStringToObject(layer, "join", "pruid");
}

void test_validate_styles(void) {
    char * json = "{\"basemap\":\"openstreetmap\",\"view\":{\"center\":[-89,57],\"zoom\":5},\"layers\":[{\"id\":\"provinces\",\"type\":\"polygon\",\"file\":\"../test/data/visualization/input/provinces.geojson\",\"label\":\"Provinces\",\"style\":\"default\",\"model\":\"pandemic\",\"join\":\"pruid\"}],\"simulation\":[{\"name\":\"recovered\",\"layer\":\"provinces\",\"fill\":{\"type\":\"quantile\",\"property\":\"recovered\",\"colors\":[\"rgba(118,42,131,0.85)\",\"rgba(153,112,171,0.85)\",\"rgba(194,165,207,0.85)\",\"rgba(231,212,232,0.85)\",\"rgba(247,247,247,0.85)\",\"rgba(217,240,211,0.85)\",\"rgba(166,219,160,0.85)\",\"rgba(90,174,97,0.85)\",\"rgba(27,120,55,0.85)\"]},\"stroke\":{\"type\":\"static\",\"width\":1,\"color\":\"rgba(0,0,0,1)\"}},{\"name\":\"population\",\"layer\":\"provinces\",\"fill\":{\"type\":\"equivalent\",\"property\":\"population\",\"colors\":[\"rgba(215,48,39,0.85)\",\"rgba(244,109,67,0.85)\",\"rgba(253,174,97,0.85)\",\"rgba(254,224,144,0.85)\",\"rgba(255,255,191,0.85)\",\"rgba(224,243,248,0.85)\",\"rgba(171,217,233,0.85)\",\"rgba(116,173,209,0.85)\",\"rgba(69,117,180,0.85)\"]},\"stroke\":{\"type\":\"static\",\"width\":1,\"color\":\"rgba(0,0,0,1)\"}},{\"name\":\"susceptible\",\"layer\":\"provinces\",\"fill\":{\"type\":\"quantile\",\"property\":\"susceptible\",\"colors\":[\"rgba(247,252,240,0.85)\",\"rgba(224,243,219,0.85)\",\"rgba(204,235,197,0.85)\",\"rgba(168,221,181,0.85)\",\"rgba(123,204,196,0.85)\",\"rgba(78,179,211,0.85)\",\"rgba(43,140,190,0.85)\",\"rgba(8,104,172,0.85)\",\"rgba(8,64,129,0.85)\"]},\"stroke\":{\"type\":\"static\",\"width\":1,\"color\":\"rgba(0,0,0,1)\"}},{\"name\":\"infected\",\"layer\":\"provinces\",\"fill\":{\"type\":\"quantile\",\"property\":\"infected\",\"colors\":[\"rgba(140,81,10,0.85)\",\"rgba(191,129,45,0.85)\",\"rgba(223,194,125,0.85)\",\"rgba(246,232,195,0.85)\",\"rgba(245,245,245,0.85)\",\"rgba(199,234,229,0.85)\",\"rgba(128,205,193,0.85)\",\"rgba(53,151,143,0.85)\",\"rgba(1,102,94,0.85)\"]},\"stroke\":{\"type\":\"static\",\"width\":1,\"color\":\"rgba(0,0,0,1)\"}}]}";
    cJSON * viz_data = cJSON_Parse(json);

    cJSON * style = cJSON_GetArrayItem(cJSON_GetObjectItem(viz_data, "simulation"), 0);

    cJSON_DeleteItemFromObject(style, "layer");
    int res = validate_visualization(viz_data);
    TEST_ASSERT_EQUAL(VIZ_SIMULATION_BAD_LAYER, res);
    cJSON_AddStringToObject(style, "layer", "provinces");

    cJSON_DeleteItemFromObject(style, "name");
    res = validate_visualization(viz_data);
    TEST_ASSERT_EQUAL(VIZ_SIMULATION_BAD_NAME, res);
    cJSON_AddStringToObject(style, "name", "recovered");
}

void test_package_visualization(void) {
    char * json = "{\"basemap\":\"openstreetmap\",\"view\":{\"center\":[-89,57],\"zoom\":5},\"layers\":[{\"id\":\"provinces\",\"type\":\"polygon\",\"file\":\"../test/data/visualization/input/provinces.geojson\",\"label\":\"Provinces\",\"style\":\"default\",\"model\":\"pandemic\",\"join\":\"pruid\"}],\"simulation\":[{\"name\":\"recovered\",\"layer\":\"provinces\",\"fill\":{\"type\":\"quantile\",\"property\":\"recovered\",\"colors\":[\"rgba(118,42,131,0.85)\",\"rgba(153,112,171,0.85)\",\"rgba(194,165,207,0.85)\",\"rgba(231,212,232,0.85)\",\"rgba(247,247,247,0.85)\",\"rgba(217,240,211,0.85)\",\"rgba(166,219,160,0.85)\",\"rgba(90,174,97,0.85)\",\"rgba(27,120,55,0.85)\"]},\"stroke\":{\"type\":\"static\",\"width\":1,\"color\":\"rgba(0,0,0,1)\"}},{\"name\":\"population\",\"layer\":\"provinces\",\"fill\":{\"type\":\"equivalent\",\"property\":\"population\",\"colors\":[\"rgba(215,48,39,0.85)\",\"rgba(244,109,67,0.85)\",\"rgba(253,174,97,0.85)\",\"rgba(254,224,144,0.85)\",\"rgba(255,255,191,0.85)\",\"rgba(224,243,248,0.85)\",\"rgba(171,217,233,0.85)\",\"rgba(116,173,209,0.85)\",\"rgba(69,117,180,0.85)\"]},\"stroke\":{\"type\":\"static\",\"width\":1,\"color\":\"rgba(0,0,0,1)\"}},{\"name\":\"susceptible\",\"layer\":\"provinces\",\"fill\":{\"type\":\"quantile\",\"property\":\"susceptible\",\"colors\":[\"rgba(247,252,240,0.85)\",\"rgba(224,243,219,0.85)\",\"rgba(204,235,197,0.85)\",\"rgba(168,221,181,0.85)\",\"rgba(123,204,196,0.85)\",\"rgba(78,179,211,0.85)\",\"rgba(43,140,190,0.85)\",\"rgba(8,104,172,0.85)\",\"rgba(8,64,129,0.85)\"]},\"stroke\":{\"type\":\"static\",\"width\":1,\"color\":\"rgba(0,0,0,1)\"}},{\"name\":\"infected\",\"layer\":\"provinces\",\"fill\":{\"type\":\"quantile\",\"property\":\"infected\",\"colors\":[\"rgba(140,81,10,0.85)\",\"rgba(191,129,45,0.85)\",\"rgba(223,194,125,0.85)\",\"rgba(246,232,195,0.85)\",\"rgba(245,245,245,0.85)\",\"rgba(199,234,229,0.85)\",\"rgba(128,205,193,0.85)\",\"rgba(53,151,143,0.85)\",\"rgba(1,102,94,0.85)\"]},\"stroke\":{\"type\":\"static\",\"width\":1,\"color\":\"rgba(0,0,0,1)\"}}]}";
    char * json_missing_layer = "{\"basemap\":\"openstreetmap\",\"view\":{\"center\":[-89,57],\"zoom\":5},\"layers\":[{\"id\":\"provinces\",\"type\":\"polygon\",\"file\":\"../test/data/visualization/input/no.geojson\",\"label\":\"Provinces\",\"style\":\"default\",\"model\":\"pandemic\",\"join\":\"pruid\"}],\"simulation\":[{\"name\":\"recovered\",\"layer\":\"provinces\",\"fill\":{\"type\":\"quantile\",\"property\":\"recovered\",\"colors\":[\"rgba(118,42,131,0.85)\",\"rgba(153,112,171,0.85)\",\"rgba(194,165,207,0.85)\",\"rgba(231,212,232,0.85)\",\"rgba(247,247,247,0.85)\",\"rgba(217,240,211,0.85)\",\"rgba(166,219,160,0.85)\",\"rgba(90,174,97,0.85)\",\"rgba(27,120,55,0.85)\"]},\"stroke\":{\"type\":\"static\",\"width\":1,\"color\":\"rgba(0,0,0,1)\"}},{\"name\":\"population\",\"layer\":\"provinces\",\"fill\":{\"type\":\"equivalent\",\"property\":\"population\",\"colors\":[\"rgba(215,48,39,0.85)\",\"rgba(244,109,67,0.85)\",\"rgba(253,174,97,0.85)\",\"rgba(254,224,144,0.85)\",\"rgba(255,255,191,0.85)\",\"rgba(224,243,248,0.85)\",\"rgba(171,217,233,0.85)\",\"rgba(116,173,209,0.85)\",\"rgba(69,117,180,0.85)\"]},\"stroke\":{\"type\":\"static\",\"width\":1,\"color\":\"rgba(0,0,0,1)\"}},{\"name\":\"susceptible\",\"layer\":\"provinces\",\"fill\":{\"type\":\"quantile\",\"property\":\"susceptible\",\"colors\":[\"rgba(247,252,240,0.85)\",\"rgba(224,243,219,0.85)\",\"rgba(204,235,197,0.85)\",\"rgba(168,221,181,0.85)\",\"rgba(123,204,196,0.85)\",\"rgba(78,179,211,0.85)\",\"rgba(43,140,190,0.85)\",\"rgba(8,104,172,0.85)\",\"rgba(8,64,129,0.85)\"]},\"stroke\":{\"type\":\"static\",\"width\":1,\"color\":\"rgba(0,0,0,1)\"}},{\"name\":\"infected\",\"layer\":\"provinces\",\"fill\":{\"type\":\"quantile\",\"property\":\"infected\",\"colors\":[\"rgba(140,81,10,0.85)\",\"rgba(191,129,45,0.85)\",\"rgba(223,194,125,0.85)\",\"rgba(246,232,195,0.85)\",\"rgba(245,245,245,0.85)\",\"rgba(199,234,229,0.85)\",\"rgba(128,205,193,0.85)\",\"rgba(53,151,143,0.85)\",\"rgba(1,102,94,0.85)\"]},\"stroke\":{\"type\":\"static\",\"width\":1,\"color\":\"rgba(0,0,0,1)\"}}]}";
    cJSON * viz_data = cJSON_Parse(json);
    cJSON * viz_data_missing_layer = cJSON_Parse(json_missing_layer);

    char output[] = "../test/data/visualization/output/";
    char output_missing_conversion[] = "../test/data/visualization/output_missing_conversion/";

    int res = package_visualization(viz_data_missing_layer, output);
    TEST_ASSERT_EQUAL(UNABLE_OPEN_FILE, res);

    res = package_visualization(viz_data, output_missing_conversion);
    TEST_ASSERT_EQUAL(UNABLE_OPEN_FILE, res);

    res = package_visualization(viz_data, output);
    TEST_ASSERT_EQUAL(SUCCESS, res);

    res = file_exists("../test/data/visualization/output/visualization/provinces.geojson");
    TEST_ASSERT_TRUE(res);

    res = file_exists("../test/data/visualization/output/visualization/visualization.json");
    TEST_ASSERT_TRUE(res);

    res = file_exists("../test/data/visualization/output/visualization/structure.json");
    TEST_ASSERT_TRUE(res);

    res = file_exists("../test/data/visualization/output/visualization/messages.log");
    TEST_ASSERT_TRUE(res);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_validate_visualization);
    RUN_TEST(test_validate_layers);
    RUN_TEST(test_validate_view);
    RUN_TEST(test_validate_styles);
    RUN_TEST(test_package_visualization);
    return UNITY_END();
}