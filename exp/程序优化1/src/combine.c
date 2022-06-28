#include<stdlib.h>
#include<stdint.h>
#include<stdio.h>
#include"combine.h"

void combine1(vec_ptr v, data_t *dest){
	long i;
	*dest = IDENT;
	for(i = 0;i<vec_length(v);i++){
		data_t val;
		get_vec_element(v, i, &val);
		*dest = *dest OP val;
	}
}

void combine2(vec_ptr v, data_t *dest){
	long i;
	long length = vec_length(v);    
    
	*dest = IDENT;
	for(i = 0;i<length;i++){
		data_t val;
		get_vec_element(v, i, &val);
		*dest = *dest OP val;
	}
}

void combine3(vec_ptr v, data_t *dest){
	long i;
	long length = vec_length(v); 
    data_t *data = get_vec_start(v);
    
	*dest = IDENT;
	for(i = 0;i<length;i++){
		*dest = *dest OP data[i];
	}
}

void combine4(vec_ptr v, data_t *dest){
	long i;
	long length = vec_length(v); 
    data_t *data = get_vec_start(v);
    data_t acc = IDENT;
    
	for(i = 0;i<length;i++){
		acc = acc OP data[i];
	}
}

void combine5(vec_ptr v, data_t *dest){
	long i;
	long length = vec_length(v);
    long limit = length-1;
    data_t *data = get_vec_start(v);
    data_t acc = IDENT;
    
	for(i = 0;i<limit;i+=2){
		acc = (acc OP data[i]) OP data[i+1];
	}
	
	for(;i<length;i++){
		acc=acc OP data[i];
	}
	*dest = acc;
}

void combine6(vec_ptr v, data_t *dest){
	long i;
	long length = vec_length(v);
    long limit = length-1;
    data_t *data = get_vec_start(v);
    data_t acc0 = IDENT;
    data_t acc1 = IDENT;
    
    
	for(i = 0;i<limit;i+=2){
		acc0 = acc0 OP data[i];
		acc1 = acc1 OP data[i];
	}
	
	for(;i<length;i++){
		acc0=acc0 OP data[i];
	}
	*dest = acc0 OP acc1;
}

void combine7(vec_ptr v, data_t *dest){
	long i;
	long length = vec_length(v);
    long limit = length-1;
    data_t *data = get_vec_start(v);
    data_t acc = IDENT;
    
	for(i = 0;i<limit;i+=2){
		acc = acc OP (data[i] OP data[i+1]);
	}
	
	for(;i<length;i++){
		acc=acc OP data[i];
	}
	*dest = acc;
}