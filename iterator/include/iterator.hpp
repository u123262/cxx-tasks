/* Image iterator.
 * @file
 * @date 2018-08-05
 * @author Anonymous
 */

#ifndef __ITERATOR_HPP__
#define __ITERATOR_HPP__

#include <algorithm>
#include <type_traits>
#include <vector>
#include <utility>

#include <boost/iterator/iterator_adaptor.hpp>

//{ image iterator
template<class Iterator>
class image_iterator: public boost::iterator_adaptor<image_iterator<Iterator>, Iterator>
{

private:
    size_t width_;
    size_t stride_;

    size_t i = 0;

    friend class boost::iterator_core_access;

    void increment()
    {
        i++;

        this->base_reference()++;

        if (i % stride_ >= width_) {
            this->base_reference() += stride_ - width_;
            i+= stride_ - width_;
        }
    }

    void decrement()
    {
        i--;

        this->base_reference()--;

        if (i % stride_ >= width_) {
            this->base_reference() -= stride_ - width_;
            i-= stride_ - width_;
        }
    }

    void advance( ptrdiff_t d )
    {
        for (int i = 0; i < +int(d); i++) {
            increment();
        }

        for (int i = 0; i < -int(d); i++) {
            decrement();
        }
    }

    ptrdiff_t distance_to( image_iterator const & element ) const
    {
        int offset = element.i % (int)stride_;
        int delta = element.base() - this->base() - offset;

        return delta / (int)stride_ * (int)width_ + delta % (int)stride_ + offset;
    }

public:
    explicit image_iterator(Iterator it, size_t width, size_t stride)
    : image_iterator::iterator_adaptor_(it), width_(width), stride_(stride) { }

};
//}

template<class Container = std::vector<uint8_t>>
class image
{
public:
    image(size_t width, size_t height, size_t stride):
        data(stride * height),
        width_(width),
        stride_(stride)
    {}

    auto begin()
    {
        return image_iterator(data.begin(), width_, stride_);
    }

    auto end()
    {
        return image_iterator(data.end(), width_, stride_);
    }

    auto begin() const
    {
        return image_iterator(data.begin(), width_, stride_);
    }

    auto end() const
    {
        return image_iterator(data.end(), width_, stride_);
    }

private:
    Container data; // image data including strides. Access by (x, y): data[y * stride_ + x]
    size_t width_;
    size_t stride_;
};

#endif // __ITERATOR_HPP__
