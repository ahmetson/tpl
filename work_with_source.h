#ifndef INIT_AND_WORK_WITH_FILES_H
#define INIT_AND_WORK_WITH_FILES_H

void work_with_sources( int argc, const char **argv );
void work_with_source( const char *parse_file_name );

void check_for_reserved_file_name( wchar_t *file_path );


void move_file_to_tmp_var( FILE *f, wchar_t **source, int *source_bytes );

#endif

