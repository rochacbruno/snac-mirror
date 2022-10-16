/* copyright (c) 2022 grunfink - MIT license */

#ifndef _XS_IO_H

#define _XS_IO_H

d_char *xs_readall(FILE *f);
d_char *xs_readline(FILE *f);
d_char *xs_read(FILE *f, int *size);


#ifdef XS_IMPLEMENTATION

d_char *xs_readall(FILE *f)
/* reads the rest of the file into a string */
{
    d_char *s;
    char tmp[1024];

    errno = 0;

    /* create the new string */
    s = xs_str_new(NULL);

    while (fgets(tmp, sizeof(tmp), f))
        s = xs_str_cat(s, tmp);

    return s;
}


d_char *xs_readline(FILE *f)
/* reads a line from a file */
{
    d_char *s = NULL;

    errno = 0;

    /* don't even try on eof */
    if (!feof(f)) {
        int c;

        s = xs_str_new(NULL);

        while ((c = fgetc(f)) != EOF) {
            unsigned char rc = c;

            s = xs_append_m(s, (char *)&rc, 1);

            if (c == '\n')
                break;
        }
    }

    return s;
}


d_char *xs_read(FILE *f, int *sz)
/* reads up to size bytes from f */
{
    d_char *s  = NULL;
    int size   = *sz;
    int rdsz   = 0;

    errno = 0;

    while (size > 0 && !feof(f)) {
        char tmp[4096];
        int n, r;

        if ((n = sizeof(tmp)) > size)
            n = size;

        r = fread(tmp, 1, n, f);

        /* open room */
        s = xs_realloc(s, rdsz + r);

        /* copy read data */
        memcpy(s + rdsz, tmp, r);

        rdsz += r;
        size -= r;
    }

    /* null terminate, just in case it's treated as a string */
    s = xs_realloc(s, rdsz + 1);
    s[rdsz] = '\0';

    *sz = rdsz;

    return s;
}

#endif /* XS_IMPLEMENTATION */

#endif /* _XS_IO_H */
