#include <iostream>
#include <vector>
#include <set>
#include <limits>
#include <cstddef>

// Access an element in a flattened square adjacency matrix.
template <typename T>
inline T& getMatrixElement(T* matrix, std::size_t size,
                           std::size_t row, std::size_t column)
{
    return *(matrix + row * size + column);
}

template <typename T>
inline const T& getMatrixElement(const T* matrix, std::size_t size,
                                 std::size_t row, std::size_t column)
{
    return *(matrix + row * size + column);
}

template <typename T>
inline void setMatrixElement(T* matrix, std::size_t size,
                             std::size_t row, std::size_t column, T element)
{
    *(matrix + row * size + column) = element;
}

// Dijkstra's algorithm using a set of {distance, vertex} pairs.
// A matrix entry of 0 means that no edge exists between two distinct vertices.
template <typename T>
std::vector<T> dijkstra(const T* matrix, std::size_t matrix_size,
                        std::size_t start_pos)
{
    const T INF = std::numeric_limits<T>::max();
    std::vector<T> result(matrix_size, INF);
    std::set<std::pair<T, std::size_t>> unsettled;

    if (start_pos >= matrix_size)
        return result;

    result[start_pos] = 0;
    unsettled.insert({0, start_pos});

    while (!unsettled.empty())
    {
        auto it = unsettled.begin();
        T current_distance = it->first;
        std::size_t current_vertex = it->second;
        unsettled.erase(it);

        // Ignore an outdated entry if a better distance was already found.
        if (current_distance != result[current_vertex])
            continue;

        for (std::size_t neighbour = 0; neighbour < matrix_size; ++neighbour)
        {
            T weight = getMatrixElement(matrix, matrix_size,
                                        current_vertex, neighbour);

            if (weight <= 0 || result[current_vertex] == INF)
                continue;

            // Avoid overflow when adding the edge weight.
            if (current_distance > INF - weight)
                continue;

            T new_distance = current_distance + weight;
            if (new_distance < result[neighbour])
            {
                if (result[neighbour] != INF)
                    unsettled.erase({result[neighbour], neighbour});

                result[neighbour] = new_distance;
                unsettled.insert({new_distance, neighbour});
            }
        }
    }

    return result;
}

int main()
{
    std::size_t size;
    std::cout << "Graph size (number of vertices): ";
    if (!(std::cin >> size) || size == 0)
    {
        std::cerr << "Please enter a positive graph size.\n";
        return 1;
    }

    std::vector<int> graph_matrix(size * size, 0);

    std::cout << "Enter the adjacency matrix (use 0 when there is no edge).\n";
    for (std::size_t row = 0; row < size; ++row)
    {
        for (std::size_t column = 0; column < size; ++column)
        {
            int weight;
            std::cout << "Weight from " << row << " to " << column << ": ";
            if (!(std::cin >> weight) || weight < 0)
            {
                std::cerr << "Invalid weight. Dijkstra requires non-negative weights.\n";
                return 1;
            }
            setMatrixElement(graph_matrix.data(), size, row, column, weight);
        }
    }

    std::size_t start_pos;
    std::cout << "Choose source vertex (0 to " << size - 1 << "): ";
    if (!(std::cin >> start_pos) || start_pos >= size)
    {
        std::cerr << "Source vertex is outside the graph.\n";
        return 1;
    }

    std::vector<int> distances = dijkstra(graph_matrix.data(), size, start_pos);

    std::cout << "Shortest distances from vertex " << start_pos << ":\n";
    for (std::size_t vertex = 0; vertex < size; ++vertex)
    {
        std::cout << "To " << vertex << ": ";
        if (distances[vertex] == std::numeric_limits<int>::max())
            std::cout << "Unreachable\n";
        else
            std::cout << distances[vertex] << '\n';
    }

    return 0;
}
