/*
*********************************************************************************************************
*                              		(c) Copyright 2020-2020, Hexin
*                                           All Rights Reserved
* File    : _canxmodule.c
* Author  : Heyn (heyunhuan@gmail.com)
* Version : V1.3
*
* LICENSING TERMS:
* ---------------
*		New Create at 	2020-04-21 [Heyn] Initialize.
*                       2020-04-27 [Heyn] Optimized Code.
*
*********************************************************************************************************
*/

#include <Python.h>
#include "_canxtables.h"

static unsigned char hexin_PyArg_ParseTuple_Paramete( PyObject *self, PyObject *args, struct _hexin_canx *param )
{
    Py_buffer data = { NULL, NULL };

#if PY_MAJOR_VERSION >= 3
    if ( !PyArg_ParseTuple( args, "y*", &data ) ) {
        if ( data.obj ) {
            PyBuffer_Release( &data );
        }
        return FALSE;
    }
#else
    if ( !PyArg_ParseTuple( args, "s*", &data ) ) {
        if ( data.obj ) {
            PyBuffer_Release( &data );
        }
        return FALSE;
    }
#endif /* PY_MAJOR_VERSION */

    param->result = hexin_canx_compute( (const unsigned char *)data.buf, (unsigned int)data.len, param  );

    if ( data.obj )
       PyBuffer_Release( &data );

    return TRUE;
}

static PyObject * _canx_can15( PyObject *self, PyObject *args )
{
    static struct _hexin_canx canx_param_can15 = { .is_initial=FALSE,
                                                   .width  = 15,
                                                   .poly   = CAN15_POLYNOMIAL_00004599,
                                                   .init   = 0x00000000L,
                                                   .refin  = FALSE,
                                                   .refout = FALSE,
                                                   .xorout = 0x00000000L,
                                                   .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &canx_param_can15 ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", (unsigned short)( canx_param_can15.result & 0x00007FFF ) );
}

static PyObject * _canx_can17( PyObject *self, PyObject *args )
{
    static struct _hexin_canx canx_param_can17 = { .is_initial=FALSE,
                                                   .width  = 17,
                                                   .poly   = CAN17_POLYNOMIAL_0001685B,
                                                   .init   = 0x00000000L,
                                                   .refin  = FALSE,
                                                   .refout = FALSE,
                                                   .xorout = 0x00000000L,
                                                   .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &canx_param_can17 ) ) {
        return NULL;
    }

    return Py_BuildValue( "I", (canx_param_can17.result & 0x1FFFF) );
}

static PyObject * _canx_can21( PyObject *self, PyObject *args )
{
    static struct _hexin_canx canx_param_can21= { .is_initial=FALSE,
                                                   .width  = 21,
                                                   .poly   = CAN21_POLYNOMIAL_00102899,
                                                   .init   = 0x00000000L,
                                                   .refin  = FALSE,
                                                   .refout = FALSE,
                                                   .xorout = 0x00000000L,
                                                   .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &canx_param_can21 ) ) {
        return NULL;
    }

    return Py_BuildValue( "I", (canx_param_can21.result & 0x001FFFFF) );
}

/* method table */
static PyMethodDef _canxMethods[] = {
    { "can15",      (PyCFunction)_canx_can15, METH_VARARGS, "Calculate CAN15 [Poly=0x4599, Init=0x0000 Xorout=0x0000 Refin=True Refout=True]" },
    { "can17",      (PyCFunction)_canx_can17, METH_VARARGS, "Calculate CAN17 [Poly=0x1685B, Init=0x0000 Xorout=0x0000 Refin=True Refout=True]" },
    { "can21",      (PyCFunction)_canx_can21, METH_VARARGS, "Calculate CAN21 [Poly=0x102899, Init=0x0000 Xorout=0x0000 Refin=True Refout=True]" },
    { NULL, NULL, 0, NULL }        /* Sentinel */
};


/* module documentation */
PyDoc_STRVAR( _canx_doc,
"Calculation of CANx \n"
"libscrc.can15   -> Calculate CAN15 [Poly = 0x4599 Initial = 0 Xorout=0 Refin=True Refout=True]\n"
"libscrc.can17   -> Calculate CAN17 [Poly = 0x1685B Initial = 0 Xorout=0 Refin=True Refout=True]\n"
"libscrc.can21   -> Calculate CAN21 [Poly = 0x102899 Initial = 0 Xorout=0 Refin=True Refout=True]\n"
"\n" );


#if PY_MAJOR_VERSION >= 3

/* module definition structure */
static struct PyModuleDef _canxmodule = {
   PyModuleDef_HEAD_INIT,
   "_canx",                    /* name of module */
   _canx_doc,                  /* module documentation, may be NULL */
   -1,                         /* size of per-interpreter state of the module */
   _canxMethods
};

/* initialization function for Python 3 */
PyMODINIT_FUNC
PyInit__canx( void )
{
    PyObject *m = NULL;

    m = PyModule_Create( &_canxmodule );
    if ( m == NULL ) {
        return NULL;
    }

    PyModule_AddStringConstant( m, "__version__", "1.3"   );
    PyModule_AddStringConstant( m, "__author__",  "Heyn"  );

    return m;
}

#else

/* initialization function for Python 2 */
PyMODINIT_FUNC
init_canx( void )
{
    (void) Py_InitModule3( "_canx", _canxMethods, _canx_doc );
}

#endif /* PY_MAJOR_VERSION */
