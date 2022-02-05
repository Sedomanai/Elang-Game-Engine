// Deprecated with cereal - used only in specific conditions

#pragma once

#include <fstream>

#include "container.h"

namespace el 
{
    using logger = std::ostream;
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


        void shift(sizet size) { f.seekp(f.tellp() + std::streampos(size)); }
        bool invalid() { return f.tellg() == -1; }
        sizet curr() { return (sizet)f.tellp(); }

        //////Save//////
        // Save string
        stream& operator<<(std::string& str) {
            sizet size = str.size();
            f.write(reinterpret_cast<const char*>(&size), sizeof(sizet));
            f.write(str.c_str(), size);
            return *this;
        }

        // Save data
        template<typename Ty_>
        std::enable_if_t<!std::is_pointer_v<Ty_ >&& std::is_pod_v<Ty_>, el::stream&>
        operator<<(Ty_ t) {
            f.write(reinterpret_cast<const char*>(&t), sizeof(Ty_));
            return (*this);
        }

        // Save array
        template<typename Ty_>
        std::enable_if_t<!std::is_pointer_v<Ty_ >&& std::is_pod_v<Ty_>, el::stream&>
        operator<<(array<Ty_> a) {
            f.write(reinterpret_cast<const char*>(a.data), sizeof(Ty_) * a.size);
            return *this;
        }

        // Save vector
        template<typename Ty_>
        std::enable_if_t<!std::is_pointer_v<Ty_ >&& std::is_pod_v<Ty_>, el::stream&>
        operator<<(const vector<Ty_>& t) {
            sizet size = t.size();
            f.write(reinterpret_cast<const char*>(&size), sizeof(sizet));
            f.write(reinterpret_cast<const char*>(&t[0]), sizeof(Ty_) * size);
            return *this;
        }

        //////Load//////

        // Load string
        stream& operator>>(std::string& str) {
            sizet size;
            f.read(reinterpret_cast<char*>(&size), sizeof(sizet));
            str.resize(size);
            f.read(&str[0], size);
            return *this;
        }

        // Load data
        template<typename Ty_>
        std::enable_if_t<!std::is_pointer_v<Ty_ >&& std::is_pod_v<Ty_>, el::stream&>
        operator>>(Ty_& t) {
            f.read(reinterpret_cast<char*>(&t), sizeof(Ty_));
            return *this;
        }

        // Load array
        template<typename Ty_>
        std::enable_if_t<!std::is_pointer_v<Ty_ >&& std::is_pod_v<Ty_>, el::stream&>
        operator>>(array<Ty_> a) {
            f.read(reinterpret_cast<char*>(a.data), sizeof(Ty_) * a.size);
            return *this;
        }

        // Load vector
        template<typename Ty_>
        std::enable_if_t<!std::is_pointer_v<Ty_ >&& std::is_pod_v<Ty_>, el::stream&>
        operator>>(vector<Ty_>& t) {
            sizet size;
            f.read(reinterpret_cast<char*>(&size), sizeof(sizet));
            t.reserve(size);
            f.read(reinterpret_cast<char*>(&t[0]), sizeof(Ty_) * size);
            t.erase(t.begin() + size, t.end());
            return *this;
        }


    private:
        std::fstream f;
    };
}
