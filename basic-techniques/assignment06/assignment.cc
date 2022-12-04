/*
 * Basics of Computer Graphics Exercise
 */
 
#include "assignment.hh"

namespace task
{

bool convex(const glm::vec2& prev, const glm::vec2& curr, const glm::vec2& next)
{
    // True iff the vertex curr is a convex corner.
    // Assume counter-clockwise vertex order.
    glm::vec2 first_edge = curr - prev;
    glm::vec2 second_edge = next - curr;
    float cross_z = (first_edge.x * second_edge.y) - (first_edge.y * second_edge.x);
    if (cross_z > 0)
        return true;
    return false;
}

bool inTriangle(const glm::vec2& p, const glm::vec2& a, const glm::vec2& b, const glm::vec2& c)
{
    // True iff the point p lies within the triangle a, b, c.
    // Assume counter-clockwise vertex order.
    glm::vec2 first_edge = b - a;
    glm::vec2 second_edge = c - b;
    glm::vec2 third_edge = a - c;
    glm::vec2 vec_first = p - a;
    glm::vec2 vec_second = p - b;
    glm::vec2 vec_third = p - c;

    float cross_first = (first_edge.x * vec_first.y) - (first_edge.y * vec_first.x);
    float cross_second = (second_edge.x * vec_second.y) - (second_edge.y * vec_second.x);
    float cross_third = (third_edge.x * vec_third.y) - (third_edge.y * vec_third.x);

    if (cross_first >= 0.0f && cross_second >= 0.0f && cross_third >= 0.0f)
        return true;
    return false;
}

bool triangleEmpty(const int i_a, const int i_b, const int i_c, const std::vector<glm::vec2>& vertices)
{
    // True iff there is no other vertex inside the triangle a, b, c.
    std::vector<int> points_in;
    for (int i = 0; i < vertices.size(); i++) {
        if (i != i_a && i != i_b && i != i_c) {
            if (inTriangle(vertices[i], vertices[i_a], vertices[i_b], vertices[i_c]))
                points_in.push_back(i);
        }
    }

    if (points_in.size() == 0)
        return true;
    return false;
}

void triangulate(const std::vector<glm::vec2>& vertices, std::vector<int>& triangles)
{
    // Loop through vertices and clip away ears until only two vertices are left.
    // Input:  "vertices" contains the polygon vertices in counter-clockwise order.
    // Output: "triangles" contains indices into the "vertices" vector. Each triplet
    //         of consecutive indices specifies a triangle in counter-clockwise order.

    size_t n = vertices.size();
    if (vertices.size() < 3)
        return;

    std::cout << "Starting triangulation of a " << n << "-gon." << std::endl;

    // True iff the vertex has been clipped.
    std::vector<bool> clipped(n, false);

    while (count(clipped.begin(), clipped.end(), false) > 2) {
        //loop through all the vertices
        for (int i = 0; i < vertices.size(); i++) {
            //ignore if already clipped
            if (clipped[i])
                continue;

            //initialize previous and next vertices for current vertex
            int i_prev = (i - 1 + vertices.size()) % vertices.size();
            int i_next = (i + 1 + vertices.size()) % vertices.size();

            //update previous and next vertices for current vertex ignoring the clipped ones
            for (int j = 1; j < vertices.size(); j++) {
                int potential_prev = (i - j + vertices.size()) % vertices.size();
                if (!clipped[potential_prev]) {
                    i_prev = potential_prev;
                    break;
                }
            }
            for (int k = 1; k < vertices.size(); k++) {
                int potential_next = (i + k + vertices.size()) % vertices.size();
                if (!clipped[potential_next]) {
                    i_next = potential_next;
                    break;
                }
            }
            
            //check conditions to clip away ears
            //std::cout << i_prev << " and " << i << " and " << i_next << std::endl;
            if (convex(vertices[i_prev], vertices[i], vertices[i_next])) {
                if (triangleEmpty(i_prev, i, i_next, vertices)) {
                    //std::cout << "clipped " << i << " with " << i_prev << i_next << std::endl;
                    clipped[i] = true;
                    triangles.push_back(i_prev);
                    triangles.push_back(i);
                    triangles.push_back(i_next);
                    break;
                }
            }
        }
    }
}

void initCustomResources()
{
}

void deleteCustomResources()
{
}

}
