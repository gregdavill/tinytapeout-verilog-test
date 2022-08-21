#include "eight_font.h"

#include "imgui.h"

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "imgui_internal.h"

typedef struct
{
  const ImVec2 *points;
  int num_points;
} segment_t;

/* "eight" vector data from https://aresluna.org/segmented-type/ */
static const ImVec2 eight_seg_a[] = {{169.6165, 4.7254}, {166.0908, 11.9206}, {162.5740, 19.0976}, {159.0571, 26.2748}, {155.5310, 33.4709}, {147.5306, 33.4770}, {139.5375, 33.4770}, {131.5393, 33.4770}, {123.5405, 33.4770}, {115.5426, 33.4770}, {107.5314, 33.4770}, {99.5386, 33.4770}, {91.5377, 33.4770}, {83.5385, 33.4770}, {75.5472, 33.4770}, {67.5363, 33.4770}, {59.6851, 34.2256}, {52.3728, 31.8570}, {44.9643, 28.8169}, {37.5688, 25.7823}, {30.1674, 22.7451}, {27.3160, 17.5719}, {31.2513, 10.6355}, {36.9992, 5.1237}, {44.1015, 1.4741}, {51.9246, 0.0213}, {59.9231, 0.0000}, {67.9261, 0.0000}, {75.9264, 0.0000}, {83.9298, 0.0000}, {91.9328, 0.0000}, {99.9292, 0.0000}, {107.9270, 0.0000}, {115.9342, 0.0000}, {123.9284, 0.0000}, {131.9417, 0.0000}, {139.9277, 0.0000}, {147.9391, 0.0000}, {155.9253, 0.0000}, {163.9333, 0.0000}, {171.9266, 0.0000}};
static const ImVec2 eight_seg_b[] = {{167.9066, 30.9961}, {171.4652, 23.8404}, {175.0294, 16.6734}, {178.5876, 9.5185}, {182.1551, 2.3449}, {188.8921, 5.6716}, {194.4567, 11.3855}, {198.1561, 18.4420}, {199.6636, 26.2756}, {199.1323, 34.2542}, {198.3016, 42.2072}, {197.4712, 50.1566}, {196.6403, 58.1124}, {195.8084, 66.0767}, {194.9775, 74.0312}, {194.1466, 81.9865}, {193.3156, 89.9427}, {192.4839, 97.9047}, {191.6528, 105.8617}, {190.8217, 113.8187}, {189.9907, 121.7746}, {189.1604, 129.7245}, {186.9112, 136.9183}, {180.7380, 142.0048}, {175.0541, 142.1278}, {169.9887, 135.9240}, {164.9282, 129.7261}, {159.8704, 123.5316}, {157.1651, 116.6252}, {157.9634, 108.6691}, {158.7618, 100.7112}, {159.5605, 92.7516}, {160.3600, 84.7840}, {161.1589, 76.8215}, {161.9570, 68.8673}, {162.7566, 60.8984}, {163.5553, 52.9387}, {164.3533, 44.9850}, {164.9093, 37.0313}};
static const ImVec2 eight_seg_c[] = {{142.4994, 262.7661}, {143.2976, 254.8115}, {144.0957, 246.8583}, {144.8947, 238.8959}, {145.6932, 230.9389}, {146.4934, 222.9651}, {147.2924, 215.0028}, {148.0903, 207.0512}, {148.8896, 199.0861}, {149.6871, 191.1383}, {150.4871, 183.1666}, {153.3068, 176.2873}, {159.5036, 171.2427}, {165.7135, 166.1875}, {171.9093, 161.1437}, {177.7630, 159.7145}, {182.8559, 165.8781}, {184.6360, 173.0366}, {183.8032, 181.0092}, {182.9737, 188.9514}, {182.1420, 196.9143}, {181.3105, 204.8745}, {180.4799, 212.8267}, {179.6483, 220.7888}, {178.8172, 228.7455}, {177.9860, 236.7036}, {177.1555, 244.6545}, {176.3235, 252.6208}, {175.4933, 260.5689}, {174.6626, 268.5220}, {173.7491, 276.4679}, {171.1863, 284.0147}, {166.5513, 290.5146}, {160.2630, 295.4098}, {152.8297, 298.3034}, {149.8467, 291.8521}, {147.5934, 284.1782}, {145.3425, 276.5123}, {143.0855, 268.8257}};
static const ImVec2 eight_seg_d[] = {{136.1244, 299.0000}, {128.1262, 299.0000}, {120.1166, 299.0000}, {112.1205, 299.0000}, {104.1148, 299.0000}, {96.1254, 299.0000}, {88.1175, 299.0000}, {80.1164, 299.0000}, {72.1163, 299.0000}, {64.1270, 299.0000}, {56.1274, 299.0000}, {48.1199, 299.0000}, {40.1174, 299.0000}, {32.1210, 299.0000}, {24.1317, 298.7934}, {16.4346, 296.6815}, {9.6869, 292.4532}, {4.4110, 286.4589}, {1.7918, 279.5688}, {9.1904, 276.5329}, {16.5886, 273.4971}, {24.0075, 270.4528}, {31.3913, 267.4230}, {38.5255, 266.6085}, {46.4914, 266.8990}, {54.4848, 266.8990}, {62.4928, 266.8990}, {70.4957, 266.8990}, {78.4843, 266.8990}, {86.4977, 266.8990}, {94.4975, 266.8990}, {102.4967, 266.8990}, {110.5039, 266.8990}, {118.4939, 266.8990}, {126.4843, 266.8990}, {133.4795, 268.2375}, {135.6881, 275.9263}, {137.9013, 283.6312}, {140.1073, 291.3110}, {142.3143, 298.9942}};
static const ImVec2 eight_seg_e[] = {{43.5376, 180.8554}, {42.7550, 188.8098}, {41.9716, 196.7730}, {41.1875, 204.7430}, {40.4041, 212.7067}, {39.6212, 220.6640}, {38.8374, 228.6310}, {38.0557, 236.5772}, {37.2720, 244.5426}, {36.4888, 252.5045}, {31.7379, 257.4988}, {24.3289, 260.4883}, {16.8953, 263.4878}, {9.4805, 266.4796}, {2.0616, 269.4731}, {0.5505, 264.5515}, {1.3806, 256.6041}, {2.2127, 248.6375}, {3.0427, 240.6916}, {3.8739, 232.7334}, {4.7045, 224.7823}, {5.5366, 216.8153}, {6.3674, 208.8614}, {7.1994, 200.8966}, {8.0298, 192.9458}, {8.8611, 184.9881}, {9.6911, 177.0415}, {10.5233, 169.0744}, {14.6451, 162.8079}, {20.6237, 157.4998}, {26.0898, 161.4894}, {31.4488, 167.4365}, {36.7962, 173.3706}, {42.1529, 179.3151}};
static const ImVec2 eight_seg_f[] = {{45.0387, 123.4259}, {39.0931, 128.7676}, {33.1425, 134.1138}, {27.1954, 139.4570}, {21.3747, 142.4311}, {16.0555, 136.4581}, {14.6874, 129.2073}, {15.5184, 121.2520}, {16.3501, 113.2894}, {17.1813, 105.3314}, {18.0122, 97.3775}, {18.8427, 89.4258}, {19.6746, 81.4616}, {20.5052, 73.5103}, {21.3368, 65.5487}, {22.1681, 57.5901}, {22.9978, 49.6466}, {23.8306, 41.6735}, {24.6602, 33.7312}, {30.2745, 33.7097}, {37.6776, 36.6969}, {45.1059, 39.6942}, {52.5298, 42.6898}, {56.6382, 47.6919}, {55.8561, 55.6423}, {55.0724, 63.6082}, {54.2893, 71.5682}, {53.5068, 79.5224}, {52.7225, 87.4939}, {51.9403, 95.4444}, {51.1562, 103.4149}, {50.3727, 111.3787}, {49.5901, 119.3336}};
static const ImVec2 eight_seg_g[] = {{145.5053, 165.9168}, {137.5147, 165.9165}, {129.5040, 165.9162}, {121.5089, 165.9159}, {113.5139, 165.9156}, {105.5070, 165.9153}, {97.5218, 165.9150}, {89.5180, 165.9147}, {81.5152, 165.9144}, {73.5093, 165.9141}, {65.5104, 165.9138}, {57.5073, 165.9135}, {49.5151, 165.9132}, {41.9574, 164.9000}, {36.5059, 159.0401}, {31.0552, 153.1809}, {31.7477, 147.6230}, {37.7289, 142.3123}, {43.7137, 136.9983}, {50.1813, 132.9720}, {58.1853, 132.9720}, {66.1972, 132.9720}, {74.1824, 132.9720}, {82.1803, 132.9720}, {90.1822, 132.9720}, {98.1830, 132.9720}, {106.1815, 132.9720}, {114.1925, 132.9720}, {122.1930, 132.9720}, {130.1917, 132.9720}, {138.1975, 132.9720}, {146.1796, 132.9720}, {154.1874, 132.9720}, {159.8183, 137.9983}, {164.9192, 144.1714}, {170.0077, 150.3294}, {164.4676, 155.5551}, {158.3570, 160.7436}, {152.2673, 165.9142}};
static const ImVec2 eight_seg_dp[] = {{222.0000, 282.4977}, {220.0988, 290.1842}, {214.8334, 296.0966}, {207.4164, 298.8863}, {199.5561, 297.9142}, {193.0452, 293.3883}, {189.3907, 286.3690}, {189.4336, 278.4393}, {193.1606, 271.4641}, {199.7215, 267.0055}, {207.5976, 266.1179}, {214.9762, 268.9847}, {220.1797, 274.9529}};

static const segment_t eight_seg[] = {
    {eight_seg_a, sizeof(eight_seg_a) / sizeof(ImVec2)},
    {eight_seg_b, sizeof(eight_seg_b) / sizeof(ImVec2)},
    {eight_seg_c, sizeof(eight_seg_c) / sizeof(ImVec2)},
    {eight_seg_d, sizeof(eight_seg_d) / sizeof(ImVec2)},
    {eight_seg_e, sizeof(eight_seg_e) / sizeof(ImVec2)},
    {eight_seg_f, sizeof(eight_seg_f) / sizeof(ImVec2)},
    {eight_seg_g, sizeof(eight_seg_g) / sizeof(ImVec2)},
    {eight_seg_dp, sizeof(eight_seg_dp) / sizeof(ImVec2)}};

void eight_font::print_segment(unsigned char c, float scale)
{
  ImDrawList *draw_list = ImGui::GetWindowDrawList();
  ImU32 colour;

  const ImVec2 segment_position = ImGui::GetCursorScreenPos();
  const ImVec2 segment_size = ImVec2(220 * scale + 5.0f, 310 * scale + 5.0f);

  /* Light up each segment */
  for (int i = 0; i < 8; i++)
  {
    colour = ImColor::HSV(0.1f, 0.1f, 0.2f);
    if (c & 1)
    {
      colour = ImColor::HSV(0.3f, 0.7f, 0.8f);
    }

    ImVec2 points[100];
    segment_t segment = eight_seg[i];

    /* Move and scale points */
    for (int idx = 0; idx < segment.num_points; idx++)
    {
      points[idx].x = (segment.points[idx].x * scale) + segment_position.x + 2.0f;
      points[idx].y = (segment.points[idx].y * scale) + segment_position.y + 2.0f;
    }

    draw_list->AddConvexPolyFilled(points, segment.num_points, colour);
    c >>= 1;
  }

  ImRect bb(segment_position, segment_position + segment_size);
  ImGui::ItemSize(segment_size, 0.0f);
  if (!ImGui::ItemAdd(bb, 0))
    return;
}
