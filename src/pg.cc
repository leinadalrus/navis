#include <cstdio>
#include <cstdlib>

#ifdef _WIN32
#include <C:\\dev\\vcpkg\\packages\\libpq_x86-windows\\include\\libpq-fe.h>
#endif

#ifdef __linux__
#include </usr/include/libpq-fe.h>
#endif

int main(int argc, char* argv[])
{
  const char* conn_info;
  PGconn* conn;
  PGresult* res;

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

  res = PQexec(
          conn,
          "explain select pg_catalog.set_config('search_path', '', false)"
        );
  if (PQresultStatus(res) != PGRES_TUPLES_OK)
  {
    PQclear(res);
    PQfinish(conn);
    return 1;
  }
  PQclear(res);

  res = PQexec(conn, "begin");
  if (PQresultStatus(res) != PGRES_COMMAND_OK)
  {
    PQclear(res);
    PQfinish(conn);
    return 1;
  }
  PQclear(res);

  res = PQexec(
          conn,
          "declare myportal cursor for select * from pg_database"
        );
  if (PQresultStatus(res) != PGRES_COMMAND_OK)
  {
    PQclear(res);
    PQfinish(conn);
    return 1;
  }
  PQclear(res);

  res = PQexec(
          conn,
          "fetch all in myportal"
        );
  if (PQresultStatus(res) != PGRES_TUPLES_OK)
  {
    PQclear(res);
    PQfinish(conn);
    return 1;
  }
  PQclear(res);

  res = PQexec(
          conn,
          "close myportal"
        );
  PQclear(res);

  res = PQexec(
          conn,
          "end"
        );
  PQclear(res);
  PQfinish(conn);
}
