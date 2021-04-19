#ifndef _JUDIDUINIO_H_
#define _JUDIDUINIO_H_

#include <ArduinoJson.h>

/* ************************************************************************** */

extern void judi_init(void);

class JUDI {
  public:
    void init(void);
    struct buffer;
};

#endif // _JUDIDUINIO_H_