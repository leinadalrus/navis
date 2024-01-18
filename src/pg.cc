#include <cstdio>
#include <cstdlib>
#include </usr/include/libpq-fe.h>

int main(int argc, char *argv[])
{
  const char *conn_info;
  PGconn *conn;
  PGresult *res;
  int fd, x, y;

  if (argc > 1)
    conn_info = argv[1];
  else
    conn_info = "dbname = postgres";

  conn = PQconnectdb(conn_info);

  if (PQstatus(conn) != CONNECTION_OK)
  {
    std::fprintf(stderr, "%s", PQerrorMessage(conn));
    PQfinish(conn);

    // NOTE(exit) std::exit(1) is-equal-to `return 1`-
    // -for main-thread process.
    // This is the only case where this occurs predictably as-designed.

    return 1;
  }
}
