#include "imgui.h"
#include <vector>
#include <map>

void testbed_run_all(void);

// Simple node structure
struct Node {
    int id;
    const char* label;
};

// Example nodes and links
static Node nodes[] = {
    {0, "Root Node"},
    {1, "Child A"},
    {2, "Child B"},
    {3, "Child C"},
    {4, "Grandchild A1"},
    {5, "Grandchild B1"},
};

static int links[][2] = {
    {0, 1},
    {0, 2},
    {0, 3},
    {1, 4},
    {2, 5},
};

// Build adjacency list
std::map<int, std::vector<int>> BuildAdjacency()
{
    std::map<int, std::vector<int>> adj;
    for (auto &link : links)
        adj[link[0]].push_back(link[1]);
    return adj;
}

void testbed_nodes1(void)
{
    ImGui::Begin("Simple Graph");

    static float spacing_x = 200.0f; // horizontal spacing between levels
    static float spacing_y = 80.0f;  // vertical spacing between siblings
    ImGui::SliderFloat("Level spacing (X)", &spacing_x, 100, 400, "%.0f");
    ImGui::SliderFloat("Sibling spacing (Y)", &spacing_y, 40, 200, "%.0f");

    ImVec2 cursor = ImGui::GetCursorScreenPos();
    cursor += ImVec2(20.f, 0.f);
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    float padding_x = 20.0f;
    float padding_y = 10.0f;

    // adjacency list
    auto adj = BuildAdjacency();

    // layout: assign levels
    std::map<int, int> level;
    level[0] = 0; // root at level 0

    // simple BFS to assign levels
    std::vector<int> queue = {0};
    while (!queue.empty()) {
        int n = queue.back();
        queue.pop_back();
        for (int child : adj[n]) {
            level[child] = level[n] + 1;
            queue.push_back(child);
        }
    }

    // group nodes by level
    std::map<int, std::vector<int>> by_level;
    for (auto &n : nodes)
        by_level[level[n.id]].push_back(n.id);

    // compute positions
    std::map<int, ImVec2> node_pos;
    for (auto &[lvl, ids] : by_level) {
        for (int i = 0; i < (int)ids.size(); i++) {
            int id = ids[i];
            ImVec2 text_size = ImGui::CalcTextSize(nodes[id].label);
            ImVec2 rect_size(text_size.x + padding_x * 2, text_size.y + padding_y * 2);

            ImVec2 pos = cursor + ImVec2(i * spacing_x, lvl * spacing_y);
            node_pos[id] = pos;

            ImVec2 rect_min = pos;
            ImVec2 rect_max = pos + rect_size;

            draw_list->AddRectFilled(rect_min, rect_max, IM_COL32(100, 150, 250, 255), 8.0f);
            draw_list->AddRect(rect_min, rect_max, IM_COL32(0, 0, 0, 255), 8.0f);

            ImVec2 text_pos = rect_min + (rect_size - text_size) * 0.5f;
            draw_list->AddText(text_pos, IM_COL32(255, 255, 255, 255), nodes[id].label);
        }
    }

    // draw links
    for (auto &link : links) {
        ImVec2 rect_size0 = ImGui::CalcTextSize(nodes[link[0]].label) * 0.5f + ImVec2(padding_x, padding_y);
        ImVec2 rect_size1 = ImGui::CalcTextSize(nodes[link[1]].label) * 0.5f + ImVec2(padding_x, padding_y);

        ImVec2 src_center = ImVec2(
            node_pos[link[0]].x + rect_size0.x,
            node_pos[link[0]].y + rect_size0.y * 2.f
        );
        ImVec2 dst_center = ImVec2(
            node_pos[link[1]].x + rect_size1.x,
            node_pos[link[1]].y + rect_size1.y * .5f
        );
        draw_list->AddLine(src_center, dst_center, IM_COL32(200, 200, 200, 255), 2.0f);
    }

    // Expand dummy to fit content
    ImGui::Dummy(ImVec2(600, 400));

    ImGui::End();}

void testbed_run_all(void)
{
    testbed_nodes1();
}
