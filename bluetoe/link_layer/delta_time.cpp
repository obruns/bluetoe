#include <bluetoe/link_layer/delta_time.hpp>
#include <ostream>
#include <cassert>

namespace bluetoe {
namespace link_layer {

    delta_time delta_time::usec( std::uint32_t usec )
    {
        return delta_time( usec );
    }

    delta_time delta_time::msec( std::uint32_t msec )
    {
        std::uint32_t usec = msec * 1000;
        assert( usec >= msec );

        return delta_time( usec );
    }

    delta_time delta_time::seconds( int s )
    {
        return delta_time( s * 1000 * 1000 );
    }

    delta_time delta_time::now()
    {
        return delta_time( 0 );
    }

    void delta_time::print( std::ostream& output ) const
    {
        if ( usec_ == 0 || usec_ % 1000 == 0 )
            output << ( usec_ / 1000 ) << "ms";
        else
            output << usec_ << "µs";
    }

    delta_time& delta_time::operator+=( const delta_time& rhs )
    {
        auto const sum = usec_ + rhs.usec_;
        assert( sum >= usec_ && sum >= rhs.usec_ );

        usec_ = sum;

        return *this;
    }

    delta_time& delta_time::operator-=( const delta_time& rhs )
    {
        auto const diff = usec_ - rhs.usec_;
        assert( diff <= usec_ );

        usec_ = diff;

        return *this;
    }

    bool delta_time::operator<( const delta_time& rhs ) const
    {
        return usec_ < rhs.usec_;
    }

    bool delta_time::operator<=( const delta_time& rhs ) const
    {
        return usec_ <= rhs.usec_;
    }

    bool delta_time::operator>( const delta_time& rhs ) const
    {
        return usec_ > rhs.usec_;
    }

    bool delta_time::operator>=( const delta_time& rhs ) const
    {
        return usec_ >= rhs.usec_;
    }

    bool delta_time::operator==( const delta_time& rhs ) const
    {
        return usec_ == rhs.usec_;
    }

    bool delta_time::operator!=( const delta_time& rhs ) const
    {
        return usec_ != rhs.usec_;
    }

    std::uint32_t delta_time::usec() const
    {
        return usec_;
    }

    std::ostream& operator<<( std::ostream& out, const delta_time& t )
    {
        t.print( out );

        return out;
    }

    delta_time operator+( delta_time lhs, delta_time rhs )
    {
        lhs += rhs;
        return lhs;
    }

    delta_time operator-( delta_time lhs, delta_time rhs )
    {
        lhs-= rhs;
        return lhs;
    }

}
}