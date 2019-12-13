### Speech Recognition Model


- final.mdl: the original kaldi model(binary format) for chinese speech recognition.
- asr_tiny.mdl: the same original kaldi model(text format) for chinese speech recognition.
- asr_tiny.onnx: the onnx format model converted from asr_tiny.mdl by kaldi-onnx tool.
- asr_tiny.conf: generated configuration file when converting kaldi model to onnx model. It contains the model's infomation for decoding.
- asr_tiny.trans: genrated when converting kaldi model to onnx model. It is the transition model of the original kaldi model. It will be used when decoding speech.
- asr_tiny.yml: the configuration yaml file for model converting in MACE.


