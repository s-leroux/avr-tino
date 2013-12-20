#include <stdint.h>

#if !defined(HAS_PRINTF)
#define HAS_PRINTF 0
#endif

#if HAS_PRINTF
#include <string.h>
#include <stdio.h>
class Log {
    public:
    Log(const char* txt, uint8_t *a, uint8_t *b) : start(a), end(b) {
        printf("%10s start = %3d, end = %3d\n", txt, *start, *end);
    }

    ~Log() {
        printf("%10s start = %3d, end = %3d\n", " -> ", *start, *end);
    }

    private:
    uint8_t *start;
    uint8_t *end;
};
#endif

class Queue {
    /**
        "Double headed" read buffer

        Empty:
        |------------------------------|
        ^                            
        |                           
      start, end

        First insert(s):
        |XXXX|YYYYYY|ZZZ|--------------|
        ^                ^  
        |                |  
        start            end

        First delete(s):
        |-----------|ZZZ|--------------|
                    ^    ^               
                    |    |               
                start    end               

        Next insert(s):
        |-----------|ZZZ|WWWWWWWWW|----|
                    ^              ^ 
                    |              |
                start             end 

        Next insert(s) - no more room:
        |HHHHHH|----|ZZZ|WWWWWWWWW|0---|
                ^   ^              ^
                |   |              |
              end   start         marker

        No more room at end:
        |HHHHHH|----|ZZZ|WWWWWWWWW|KKKK|
                ^   ^             
                |   |            
              end   start       
    */

    public:
    Queue() : start(0), end(0) {
    }

    void* first() const volatile {
        if (start != end) {
            Blk* blk = (Blk*)&buffer[start];
            return blk->data;
        }

        return NULL;
    }

    uint8_t size(const void* ptr) const {
        const Blk* blk = ((const Blk*)ptr)-1;

        return blk->size;
    }

    void next() volatile {
        free();
    }

    void discard(void* ptr) {
#if HAS_PRINTF
        Log log("discard()", &start, &end);
#endif

        /* !!! Assume ptr being the *last* allocated block !!! */
        Blk *blk = ((Blk*)ptr)-1;
        end = ((uint8_t*)blk)-buffer;
    }

    void* alloc(uint8_t size) {
#if HAS_PRINTF
        Log log("alloc()", &start, &end);
#endif

        size += sizeof(Blk);
        uint8_t nend = end+size;

        if (nend < end) {
            /* not enought room at the end. Maybe at start? */
            if (start > size) {
                /* head allocation and mark unused end */
                if (start == end) {
                    start = 0; 
                }
                else {
                    buffer[end] = 0;
                }

                Blk* blk = (Blk*)&buffer[0];
                blk->size = size;
                end = size;
                return blk->data;
            }
            else {
                return NULL;
            }
        }

        if ((nend < start) || (start <= end)) {
            Blk* blk = (Blk*)&buffer[end];
            blk->size = size;
            end = nend;
            return blk->data;
        }

        return NULL;
    }

    protected:
    struct Blk {
        uint8_t size;
        uint8_t data[0];
    };

    void free() volatile {
#if HAS_PRINTF
        Log log("free()", &start, &end);
#endif

        if (start != end) {
            Blk* blk = (Blk*)&buffer[start];
            start += blk->size;

            if ((end < start) && (buffer[start] == 0))
                start = 0;
        }
    }

    private:
    uint8_t     buffer[256];
    uint8_t     start;
    uint8_t     end;
};

#if HAS_PRINTF
void* append(Queue& q, const char* str) {
    void* blk = q.alloc(strlen(str)+1);
    if (blk) {
        strcpy((char*)blk, str);
    }
    return blk;
}

int main() {
    Queue reader;

    printf("1\n");
    append(reader, "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
    printf("2\n");
    printf("%s\n", reader.first());
    reader.next();
    void* ptr = append(reader, "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB");
    reader.discard(ptr);
    append(reader, "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC");
    append(reader, "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD");
    append(reader, "EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE");
    append(reader, "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");

    void * data;
    while((data = reader.first()) != NULL) {
        printf("%s\n", data);
        reader.next();
    }

    printf("1\n");
    append(reader, "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
    append(reader, "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB");
    printf("2\n");
    printf("%s\n", reader.first());
    reader.next();
    printf("%s\n", reader.first());
    reader.next();
    append(reader, "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC");
    append(reader, "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD");
    append(reader, "EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE");
    append(reader, "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");

    while((data = reader.first()) != NULL) {
        printf("%s\n", data);
        reader.next();
    }
}
#endif
