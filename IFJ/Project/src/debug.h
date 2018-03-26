/**
 * Name: Implementace překladace imperativního jazyka IFJ17
 * Group: 48
 * Year: 2017/2018
 * University: VUTBR FIT
 * Logins: xblask04
 * Names: Barbora Blašková
 */
/*
 * Simple function for debug print. 
 */
#ifndef DEBUG_H
#define DEBUG_H

#define debug_print(...) \
            do { if (DEBUG) fprintf(stderr, "[DEBUG] "__VA_ARGS__); } while (0)

#endif