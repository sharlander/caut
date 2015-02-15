#ifndef STRUCTS_H
#define STRUCTS_H
#endif

struct file_data {
    char name[200];
    char ensure[200];
    char path[200];
    char owner[200];
    char group[200];
    char mode[200];
} file;

struct packge_data {
    char name[200];
    char ensure[200];
} package;

struct service_data {
    char name[200];
    char ensure[200];
    char enable[200];
} service;
