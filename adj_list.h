#ifndef ADJ_LIST
#define ADJ_LIST

#include <memory>

template <typename T>
class var_arr
{
    private:

        static const int init_size = 15;
        int size = init_size;
        int last = -1;

        std::unique_ptr<T[]> D = std::unique_ptr<T[]>(new T[init_size]);

        void copy(int oldsz)
        {
            std::unique_ptr<T[]> tmp = std::move(this->D);
            this->D = std::unique_ptr<T[]>(new T[this->size]);

            for ( int i = 0; i < oldsz - 1; i++)
                this->D[i] = tmp[i];
        }

    public:

        int length()
        {
            return ((this->last) + 1);
        }

        T& operator [] (int i)
        {
            int oldsz = this->size;

            while ( i > this->size ) {
                this->size += init_size;

                if ( this->size > i ) {
                    this->last = i;
                    this->copy(oldsz);
                } else this->last = this->size - 1;
            };

            if ( i > this->last) this->last = i;

            return this->D[i];
        }

        T& push(T value)
        {
            this->D[last + 1] = value;
            this->last++;
            return this->D[this->last];
        }

        T& pop()
        {
            last--;
            return this->D[last + 1];
        }
};

template <typename T>
concept Comparable = requires (T a, T b)
{
    a == b;
};

template<Comparable T, Comparable U = int>
class graph
{
    private:
        class vertex
        {
            public:
                graph* parent;
                T data;
                var_arr<int> adjacent;

                vertex() { };
                vertex(const vertex& in)
                {
                    this->data = in.data;
                };

                void operator = (const vertex& vert)
                {
                    this->data = vert.data;
                    this->parent = vert.parent;
                };

                void operator = (int value)
                {
                    this->data = value;
                };

                T& value()
                {
                    return this->data;
                };

                vertex& operator [] (int i)
                {
                    int edge = adjacent[i];
                    int index = (this->parent)->edges[edge].to;
                    return (this->parent)->operator[](index);
                };

                void setParent( graph* p)
                {
                    this->parent = p;
                };

                friend std::ostream& operator<< (std::ostream& os, const vertex& v)
                {
                    os << v.data;
                    return os;
                };

        };

        class edge
        {
            public:
                U data;
                int from;
                int to;

                void operator = (U value)
                {
                    this->data = value;
                };

                U& value()
                {
                    return this->data;
                };

                friend std::ostream& operator<<(std::ostream& os, const edge& e)
                {
                    os << e.data;
                    return os;
                };
        };



        var_arr<vertex> vertices;
        var_arr<edge> edges;

    public:

        int insertVertex(T data)
        {
            int index = findVertex(data);

            if ( index != -1) return index;

            vertex tmp;
            tmp.data = data;
            vertices.push(tmp);
            vertices[vertices.length() - 1].setParent(this);
            return vertices.length() - 1;
        };

        int insertAdjacent(T rdata, T data, U edgeweight)
        {
            int root = insertVertex(rdata);
            int index = insertVertex(data);
            int edge = this->operator()(edgeweight, root, index);
            vertices[root].adjacent.push(edge);
            return index;
        };

        int findVertex(T data)
        {
            int length = vertices.length();

            for ( int i = 0; i < length; i++) {
                if ( vertices[i].data == data ) return i;
            };

            return -1; // should throw execption
        }

        T getAdjacent(int root, int index)
        {
            int edge = vertices[root].adjacent[index];
            return vertices[edges[edge].to].data;
        };

        vertex& operator [] (int i)
        {
            return vertices[i];
        };

        edge& operator () (T from, T to)
        {
            int root = findVertex(from);
            int index = findVertex(to);
            int edge = vertices[root].adjacent[index];
            vertices[root].adjacent.push(edge);
            return edges[edge];
        };

        int operator () (int from, int to, U edgeWeight)
        {
            edge tmp;
            tmp.data = edgeWeight;
            tmp.from = from;
            tmp.to = to;
            edges.push(tmp);
            return edges.length() - 1;
        };

        int operator += (T value)
        {
            int index = findVertex(value);

            if ( index != -1) return index;

            vertex tmp;
            tmp.data = value;
            vertices.push(tmp);
            vertices[vertices.length() - 1].setParent(this);
            return vertices.length() - 1;
        };
};

#endif
