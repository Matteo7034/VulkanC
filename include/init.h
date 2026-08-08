#ifndef INIT_H
#define INIT_H
#include <stdbool.h>
#include "app.h"
typedef struct App App;

bool create_Instance(App *app);

void check_available_extensions(void);

#endif
