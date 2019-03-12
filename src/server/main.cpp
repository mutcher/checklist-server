#include "RestApiService.h"

int main(int argc, char** argv) {
  const uint16_t service_port = 9090;

  RestApiService service(service_port);
  service.run();

  return 0;
}
