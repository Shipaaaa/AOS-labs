//
// Created by Vladislav Shipugin on 12.10.19.
//

/*
7. Написать программу для получения справочной информации о файлах 
с помощью системного вызова stat. 
Имя файла передается в программу из командной строки. 

Получить справочную информацию для различных типов файлов 
(обычный файл, каталог, специальные байт- и блок-ориентированные файлы).
*/
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/errno.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

void change_printf_to_error() {
    printf("\033[0;31m");
}

void change_printf_to_log() {
    printf("\033[0m");
}

int main(const int argc, const char *argv[]) {
    errno = 0;

    if (argc < 2) {
        change_printf_to_error();
        printf("Введите имя файла.\n");
        change_printf_to_log();
        return 0;
    }

    const char *file_name = argv[1];
    struct stat stat_buffer;

    stat(file_name, &stat_buffer);

    switch (stat_buffer.st_mode & S_IFMT) {
        case S_IFBLK:
            printf("block special\n");
            break;

        case S_IFCHR:
            printf("character special\n");
            break;

        case S_IFDIR:
            printf("directory\n");
            break;

        case S_IFIFO:
            printf("named pipe (fifo)\n");
            break;

        case S_IFLNK:
            printf("symbolic link\n");
            break;

        case S_IFREG:
            printf("regular\n");
            break;

        case S_IFSOCK:
            printf("socket\n");
            break;

        default:
            printf("unknown?\n");
            break;
    }

    printf("Тип файла и права доступа: %lo\n", (unsigned long)stat_buffer.st_mode);
    printf("Номер индексного дескриптора. Поля st_ino и st_dev однозначно определяют обычные файлы: %ld\n", (long)stat_buffer.st_ino);
    printf("Номер устройства, на котором расположен файл (номер уст­ройства файловой системы): %ld\n", (long)stat_buffer.st_dev);
    printf("Для специального файла устройства содержит номер устрой­ ства, адресуемого этим файлом: %ld\n", (long)stat_buffer.st_rdev);
    printf("Число жестких связей: %ld\n", (long)stat_buffer.st_nlink);
    printf("Идентификатор пользователя-владельца файла: %ld\n", (long)stat_buffer.st_uid);
    printf("Идентификатор группы-владельца файла: %ld\n", (long)stat_buffer.st_gid);
    printf("Размер файла в байтах. Для специальных файлов устройств это поле не определено: %lld\n", (long long)stat_buffer.st_size);
    printf("Оптимальный размер блока для операций ввода-вывода. Для специальных файлов устройств и каналов это поле не опре­делено: %ld\n", (long)stat_buffer.st_blksize);
    printf("Число размещенных блоков хранения данных. Для специаль­ ных файлов устройств это поле не определено: %lld\n", (long long)stat_buffer.st_blocks);
    printf("Время последнего доступа к файлу: %ld\n", (long)stat_buffer.st_atime);
    printf("Время последней модификации данных файла: %ld\n", (long)stat_buffer.st_mtime);
    printf("Время последней модификации метаданных файла: %ld\n", (long)stat_buffer.st_ctime);

    return 0;
}
