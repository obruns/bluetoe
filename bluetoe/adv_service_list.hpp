#ifndef BLUETOE_ADV_SERVICE_LIST_HPP
#define BLUETOE_ADV_SERVICE_LIST_HPP

#include <bluetoe/codes.hpp>
#include <bluetoe/bits.hpp>

namespace bluetoe {

    namespace details {
        struct list_of_16_bit_service_uuids_tag {};
    }

    /**
     * @brief complete list of 16 bit service UUIDs to be added to the advertising data
     */
    template < typename ... UUID16 >
    struct list_of_16_bit_service_uuids {
        /** @cond HIDDEN_SYMBOLS */
        using meta_type = details::list_of_16_bit_service_uuids_tag;

        static std::uint8_t* advertising_data( std::uint8_t* begin, std::uint8_t* end )
        {
            const std::size_t buffer_size = end - begin;

            if ( buffer_size < 4 )
                return begin;

            const std::size_t max_uuids = std::min( (buffer_size - 2) / 2, sizeof ...(UUID16));

            *begin = 1 + 2 * max_uuids;
            ++begin;

            *begin = max_uuids == sizeof ...(UUID16)
                ? bits( details::gap_types::complete_service_uuids_16 )
                : bits( details::gap_types::incomplete_service_uuids_16 );
            ++begin;

            for ( std::size_t uuid = 0; uuid != max_uuids; ++uuid )
            {
                begin = details::write_16bit( begin, values_[ uuid ] );
            }

            return begin;
        }

        static constexpr std::uint16_t values_[ sizeof ...(UUID16) ] = { UUID16::as_16bit()...};
        /** @endcond */
    };

    /** @cond HIDDEN_SYMBOLS */
    template < typename ... UUID16 >
    constexpr std::uint16_t list_of_16_bit_service_uuids< UUID16... >::values_[ sizeof ...(UUID16) ];
    /** @endcond */

    template <>
    struct list_of_16_bit_service_uuids<> {
        /** @cond HIDDEN_SYMBOLS */
        using meta_type = details::list_of_16_bit_service_uuids_tag;

        static std::uint8_t* advertising_data( std::uint8_t* begin, std::uint8_t* )
        {
            return begin;
        }
        /** @endcond */
    };

    struct no_list_of_service_uuids {
        /** @cond HIDDEN_SYMBOLS */
        using meta_type = details::list_of_16_bit_service_uuids_tag;

        static std::uint8_t* advertising_data( std::uint8_t* begin, std::uint8_t* )
        {
            return begin;
        }
        /** @endcond */
    };

    namespace details {

    }
}

#endif
