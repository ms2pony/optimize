typedef long data_t;
typedef struct {
	long len;
	data_t *data;
} vec_rec, *vec_ptr;

#define IDENT 0
#define OP +

data_t* get_vec_start(vec_ptr v);
long vec_length(vec_ptr v);
void combine1(vec_ptr v, data_t *dest);
void combine2(vec_ptr v, data_t *dest);
void combine3(vec_ptr v, data_t *dest);
void combine4(vec_ptr v, data_t *dest);
void combine5(vec_ptr v, data_t *dest);
void combine6(vec_ptr v, data_t *dest);
void combine7(vec_ptr v, data_t *dest);



int get_vec_element(vec_ptr v, long index, data_t *dest);