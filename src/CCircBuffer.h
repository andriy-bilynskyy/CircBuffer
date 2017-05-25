#ifndef CCIRCBUFFER_H_
#define CCIRCBUFFER_H_

#include <stdint.h>
#include <new>
#include <algorithm>

/**
 * FIFO buffer for storing Data_t elements.
 */
template <class Data_t>
class CCircBuffer
{
public:

    /**
     * Create buffer.
     * @param size - size of buffer in elements
     * @param overWriteEn - overwrite old data with new when overflow
     */
    CCircBuffer(uint32_t size, bool overWriteEn) : m_dataSize(size),
                                                   m_overWriteEn(overWriteEn)
    {
        m_pData = new (std::nothrow) Data_t[size];
        m_dataHead = 0;
        m_dataTail = 0;
        m_isEmpty = true;
    }

    /**
     * Destroy buffer.
     */
    ~CCircBuffer()
    {
        delete [] m_pData;
    }

    /**
     * Get buffer size in elements.
     * @return buffer size
     */
    uint32_t size() const
    {
        return m_pData ? m_dataSize : 0;
    }

    /**
     * Get elements inside the buffer.
     * @return - elements count
     */
    uint32_t used() const
    {
        uint32_t used = 0;

        if(m_pData)
        {
            if(m_dataHead > m_dataTail)
            {
                used = m_dataHead - m_dataTail;
            }
            else if (m_dataHead < m_dataTail)
            {
                used = m_dataSize - m_dataTail + m_dataHead;
            }
            else if(!m_isEmpty)
            {
                used = m_dataSize;
            }
        }

        return used;
    }

    /**
     * Get free buffer size in elements.
     * @return free buffer size
     */
    uint32_t free() const
    {
        return size() - used();
    }

    /**
     * Clear buffer
     */
    void clear()
    {
        m_isEmpty = true;
        m_dataHead = 0;
        m_dataTail = 0;
    }

    /**
     * Put data into buffer.
     * @param data - pointer to data
     * @param dataSize - data size
     * @return
     */
    uint32_t put(const Data_t data[], uint32_t dataSize)
    {
        uint32_t wrSize = 0;

        if(m_pData && data)
        {
            while(dataSize && (m_overWriteEn || !isFull()))
            {
                uint32_t btc = 0;
                if(m_dataHead >= m_dataTail)
                {
                    btc = m_dataSize - m_dataHead;
                }
                else
                {
                    btc = m_dataTail - m_dataHead;
                }
                if(btc > dataSize)
                {
                    btc = dataSize;
                }
                std::copy(&data[0], &data[btc], &m_pData[m_dataHead]);
                dataSize -= btc;
                data += btc;
                wrSize += btc;
                if(m_dataHead + btc == m_dataSize)
                {
                    if(isFull())
                    {
                        m_dataTail = 0;
                    }
                    m_dataHead = 0;
                }
                else
                {
                    if(isFull())
                    {
                        m_dataTail += btc;
                    }
                    m_dataHead += btc;
                }
                m_isEmpty = false;
            }
        }

        return wrSize;
    }

    /**
     * Obtain data from buffer.
     * @param data - buffer to obtained data
     * @param bufSize - buffer size
     * @return obtained elements
     */
    uint32_t get(Data_t data[], uint32_t bufSize)
    {
        uint32_t rdSize = 0;

        if(m_pData && data)
        {
            while(bufSize && !isEmpty())
            {
                uint32_t btc = 0;
                if(m_dataTail >= m_dataHead)
                {
                    btc = m_dataSize - m_dataTail;
                }
                else
                {
                    btc = m_dataHead - m_dataTail;
                }
                if(btc > bufSize)
                {
                    btc = bufSize;
                }
                std::copy(&m_pData[m_dataTail], &m_pData[m_dataTail + btc],
                          &data[0]);
                bufSize -= btc;
                data += btc;
                rdSize += btc;
                if(m_dataTail + btc == m_dataSize)
                {
                    m_dataTail = 0;
                }
                else
                {
                    m_dataTail += btc;
                }
                if(m_dataTail == m_dataHead)
                {
                    m_isEmpty = true;
                }
            }
        }

        return rdSize;
    }

    /**
     * Check data at index position. No getting data just check.
     * @param idx - index
     * @return data at indexed position
     */
    Data_t operator [](uint32_t idx) const
    {
        Data_t res(0);

        if(m_pData)
        {
            if(m_dataTail < m_dataHead)
            {
                if(m_dataTail + idx < m_dataHead)
                {
                    res = m_pData[m_dataTail + idx];
                }
            }
            else if(!isEmpty())
            {
                if(m_dataTail + idx < m_dataSize)
                {
                    res = m_pData[m_dataTail + idx];
                }
                else if(idx + m_dataTail - m_dataSize < m_dataHead)
                {
                    res = m_pData[idx + m_dataTail - m_dataSize];
                }
            }
        }

        return res;
    }

    /**
     * Check data array starting from position. No getting data just check.
     * @param idx - start data position
     * @param data - buffer to copy data
     * @param bufSize - buffer size
     * @return obtained elements count
     */
    uint32_t checkAt(uint32_t idx, Data_t data[], uint32_t bufSize) const
    {
        uint32_t rdSize = 0;

        if(m_pData && data)
        {
            if(m_dataTail < m_dataHead)
            {
                if(m_dataTail + idx < m_dataHead)
                {
                    rdSize = m_dataHead - m_dataTail - idx;
                    if(rdSize > bufSize)
                    {
                        rdSize = bufSize;
                    }
                    std::copy(&m_pData[m_dataTail + idx],
                              &m_pData[m_dataTail + idx + rdSize], &data[0]);
                }
            }
            else if(!isEmpty())
            {
                if(m_dataTail + idx < m_dataSize)
                {
                    rdSize = m_dataSize - m_dataTail - idx;
                    if(rdSize > bufSize)
                    {
                        rdSize = bufSize;
                    }
                    std::copy(&m_pData[m_dataTail + idx],
                              &m_pData[m_dataTail + idx + rdSize], &data[0]);
                    bufSize -= rdSize;
                    data += rdSize;
                    rdSize += m_dataHead > bufSize ? bufSize : m_dataHead;
                    std::copy(&m_pData[0],
                              &m_pData
                                  [m_dataHead > bufSize ? bufSize : m_dataHead],
                              &data[0]);
                }
                else if(idx + m_dataTail - m_dataSize < m_dataHead)
                {
                    rdSize = m_dataHead + m_dataSize - idx - m_dataTail;
                    if(rdSize > bufSize)
                    {
                        rdSize = bufSize;
                    }
                    std::copy(&m_pData[idx + m_dataTail - m_dataSize],
                              &m_pData[idx + m_dataTail - m_dataSize + rdSize],
                              &data[0]);
                }
            }
        }

        return rdSize;
    }

private:

    Data_t          * m_pData;
    const uint32_t    m_dataSize;
    uint32_t          m_dataHead;
    uint32_t          m_dataTail;
    bool              m_isEmpty;
    const bool        m_overWriteEn;

    bool isEmpty() const
    {
        return (m_dataHead == m_dataTail && m_isEmpty);
    }

    bool isFull() const
    {
        return (m_dataHead == m_dataTail && !m_isEmpty);
    }

    CCircBuffer & operator=(const CCircBuffer &);
    CCircBuffer(const CCircBuffer &);
};

#endif
