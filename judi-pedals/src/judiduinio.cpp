#include "judiduino.hpp"

/* ************************************************************************** */

class JudiBuffer {
  public:
    char data[256];
    uint16_t length = 0;
    uint16_t depth = 0;

    JudiBuffer(void) {
        clear(); //
    }

    void clear(void) {
        memset(&data, 0, sizeof(data));
        length = 0;
        depth = 0;
    }

    void insert(char currentChar) {
        if (currentChar == '{') {
            depth++;
        }
        if (depth > 0) {
            data[length++] = currentChar;
        }
        if (currentChar == '}') {
            depth--;
        }
    }
};

class JUDI {
  public:
    JudiBuffer buffer;

    void judi_init(void) {
        buffer.clear(); //
    }

    void clear_buffer(void) {
        memset(&buffer, 0, sizeof(buffer)); //
    }
};