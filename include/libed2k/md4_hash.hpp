
#ifndef __LIBED2K_MD4_HASH__
#define __LIBED2K_MD4_HASH__

#include <string>
#include <sstream>
#include <string.h>
#include <algorithm>
#include <typeinfo>
#include <vector>

#include <boost/cstdint.hpp>
#include <boost/assert.hpp>
#include <boost/optional.hpp>

#include <libed2k/bitfield.hpp>
#include <libtorrent/escape_string.hpp>

#include "libed2k/log.hpp"
#include "libed2k/archive.hpp"
#include "libed2k/error_code.hpp"
#include "libed2k/types.hpp"

namespace libed2k{

    const size_t MD4_HASH_SIZE = 16;

    /**
      * this class simple container for hash array
     */
    class md4_hash
    {
    public:
        friend class archive::access;
        typedef boost::uint8_t md4hash_container[MD4_HASH_SIZE];
        static const md4_hash terminal;
        static const md4_hash libed2k;
        static const md4_hash emule;

        enum { hash_size = MD4_HASH_SIZE };

    	md4_hash()
    	{
    	    clear();
    	}

    	md4_hash(const std::vector<boost::uint8_t>& vHash)
    	{
            size_t nSize = (vHash.size()> MD4_HASH_SIZE)?MD4_HASH_SIZE:vHash.size();

    		for (size_t i = 0; i < nSize; i++)
    		{
    			m_hash[i] = vHash[i];
    		}
    	}

    	md4_hash(const md4hash_container& container)
    	{
    		memcpy(m_hash, container, MD4_HASH_SIZE);
    	}

        bool defined() const
        {
            int sum = 0;
            for (size_t i = 0; i < MD4_HASH_SIZE; ++i)
                sum |= m_hash[i];
            return sum != 0;
        }

    	unsigned char* getContainer()
    	{
    	    return &m_hash[0];
    	}

    	bool operator==(const md4_hash& hash) const
        {
    	    return (memcmp(m_hash, hash.m_hash, MD4_HASH_SIZE) == 0);
        }

    	bool operator!=(const md4_hash& hash) const
        {
    	    return (memcmp(m_hash, hash.m_hash, MD4_HASH_SIZE) != 0);
        }

    	bool operator<(const md4_hash& hash) const
    	{
    	    return (memcmp(m_hash, hash.m_hash, MD4_HASH_SIZE) < 0);
    	}

    	bool operator>(const md4_hash& hash) const
    	{
    	     return (memcmp(m_hash, hash.m_hash, MD4_HASH_SIZE) > 0);
    	}

    	void clear()
    	{
    	    memset(m_hash, 0, MD4_HASH_SIZE);
    	}

    	static md4_hash fromString(const std::string& strHash)
    	{
    	    BOOST_ASSERT(strHash.size() == MD4_HASH_SIZE*2);

    	    md4_hash hash;

    	    if (!libtorrent::from_hex(strHash.c_str(), MD4_HASH_SIZE*2, (char*)hash.m_hash))
    	    {
    	        return (md4_hash());
    	    }

    	    return (hash);
    	}

    	std::string toString() const
    	{
    	    std::stringstream ss;

    	    for (size_t i = 0; i < MD4_HASH_SIZE; i++)
    	    {
    	        ss << std::uppercase << std::hex << (m_hash[i] >> 4) << (m_hash[i] & 0x0F);
    	    }

    	    return (ss.str());
    	}

    	boost::uint8_t operator[](size_t n) const
    	{
    	    if (n >= MD4_HASH_SIZE)
            {
                throw libed2k_exception(errors::md4_hash_index_error);
            }

    	    return (m_hash[n]);
    	}

    	boost::uint8_t& operator[](size_t n)
        {
    	    if (n >= MD4_HASH_SIZE)
            {
                throw libed2k_exception(errors::md4_hash_index_error);
            }
    	    return (m_hash[n]);
        }

    	template<typename Archive>
    	void serialize(Archive& ar)
    	{
    	    for (size_t n = 0; n < sizeof(md4hash_container); n++)
    	    {
    	        ar & m_hash[n];
    	    }
    	}

    	void dump() const
    	{
    	    DBG("md4_hash::dump " << toString().c_str());
    	}

    	/**
    	  * for using in logger output
    	 */
    	friend std::ostream& operator<< (std::ostream& stream, const md4_hash& hash);
    private:
    	md4hash_container   m_hash;
    };
}

#endif
