from ctypes import *
import sbn_python_client as sbn


# these classes were generated with ChatGPT
class SAMPLE_APP_HkTlm_Payload_t(Structure):
    _pack_ = 1
    _fields_ = [
        ("CommandErrorCounter", c_uint8),
        ("CommandCounter", c_uint8),
        ("spare", c_uint8 * 2)       
    ]

class SAMPLE_APP_HkTlm_t(Structure):
    _pack_ = 1
    _fields_ = [
        ("TlmHeader", sbn.CFE_SB_Msg_t),
        ("Payload", SAMPLE_APP_HkTlm_Payload_t)
    ]