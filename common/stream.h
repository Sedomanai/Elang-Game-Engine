/*****************************************************************//**
 * @file   stream.h
 * @brief  For bitstreaming i.e. serializing. 
 *         Mostly replaced by cereal, used only in specific conditions 
 *         (Exporting serialized data to other languages like C# for exmaple)
 * 
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/
#pragma once
#include <fstream>
#include <vector>

namespace el 
{
    template <typename T>
    using vector = std::vector<T>;
    template<typename Ty_>
    struct array { Ty_* data; size_t size; };

    struct stream {
        stream() {}

        bool toSave(const char* filename) {
            f.open(filename, std::ios::out | std::ios::binary);
            return f.good();
        }
        bool toLoad(const char* filename) {
            f.open(filename, std::ios::in | std::ios::binary);
            return f.good();
        }
        void close() {
            f.flush();
            f.close();
        }

        void save(void* ptr, sizet size) {
            f.write(reinterpret_cast<const char*>(ptr), size);
        }

        void load(void* ptr, sizet size) {
            f.read(reinterpret_cast<char*>(ptr), size);
        }


        void skip(sizet size) { f.seekp(f.tellp() + std::streampos(size)); }
        bool invalid() { return f.tellg() == -1; }
        sizet curr() { return (sizet)f.tellp(); }

        //////Save//////
        // Save data
        template<typename T>
        std::enable_if_t<!std::is_pointer_v<T >&& std::is_pod_v<T>, el::stream&>
            operator<<(T t) {
            f.write(reinterpret_cast<const char*>(&t), sizeof(T));
            return (*this);
        }

        // Save array (no count)
        template<typename T>
        std::enable_if_t<!std::is_pointer_v<T >&& std::is_pod_v<T>, el::stream&>
            operator<<(array<T> a) {
            f.write(reinterpret_cast<const char*>(a.data), sizeof(T) * a.size);
            return *this;
        }

        // Save string
        stream& operator<<(const std::string& str) {
            int32 size = (int32)str.size();
            f.write(reinterpret_cast<const char*>(&size), sizeof(int32));
            f.write(str.c_str(), size);
            return *this;
        }

        // Save vector
        template<typename T>
        std::enable_if_t<!std::is_pointer_v<T >&& std::is_pod_v<T>, el::stream&>
        operator<<(const vector<T>& t) {
            int32 size = (int32)t.size();
            f.write(reinterpret_cast<const char*>(&size), sizeof(int32));
            f.write(reinterpret_cast<const char*>(&t[0]), sizeof(T) * size);
            return *this;
        }

        // Save vector of recursive data
        template<typename T>
        el::stream& operator<<(const vector<T>& t) {
            int32 size = (int32)t.size();
            f.write(reinterpret_cast<const char*>(&size), sizeof(int32));
            for (sizet i = 0; i < size; i++) {
                operator<<(t[i]);
            }
            return *this;
        }

        //////Load//////

        // Load string
        stream& operator>>(std::string& str) {
            int32 size;
            f.read(reinterpret_cast<char*>(&size), sizeof(int32));
            str.resize((sizet)size);
            f.read(&str[0], (std::streamsize)size);
            return *this;
        }

        // Load data
        template<typename T>
        std::enable_if_t<!std::is_pointer_v<T >&& std::is_pod_v<T>, el::stream&>
        operator>>(T& t) {
            f.read(reinterpret_cast<char*>(&t), sizeof(T));
            return *this;
        }

        // Load array
        template<typename T>
        std::enable_if_t<!std::is_pointer_v<T >&& std::is_pod_v<T>, el::stream&>
        operator>>(array<T> a) {
            f.read(reinterpret_cast<char*>(a.data), sizeof(T) * a.size);
            return *this;
        }

        // Load vector
        template<typename T>
        std::enable_if_t<!std::is_pointer_v<T >&& std::is_pod_v<T>, el::stream&>
        operator>>(vector<T>& t) {
            int32 size;
            f.read(reinterpret_cast<char*>(&size), sizeof(int32));
            t.reserve((sizet)size);
            f.read(reinterpret_cast<char*>(&t[0]), sizeof(T) * (std::streamsize)size);
            t.erase(t.begin() + (std::streamsize)size, t.end());
            return *this;
        }

        // Load vector of recursive data
        template<typename T>
        el::stream& operator>>(vector<T>& t) {
            int32 size;
            f.read(reinterpret_cast<char*>(&size), sizeof(int32));
            t.reserve((sizet)size);
            for (sizet i = 0; i < size; i++) {
                auto& data = t.emplace_back();
                operator>>(data);
            }
            return *this;
        }

    private:
        std::fstream f;
    };
}
