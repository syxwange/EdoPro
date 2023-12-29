//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.md file in the project root for full license information.
//


// <code>
#include <iostream>
#include <speechapi_cxx.h>

using namespace std;
using namespace Microsoft::CognitiveServices::Speech;
using namespace Microsoft::CognitiveServices::Speech::Audio;

void synthesizeSpeech()
{

    auto config = SpeechConfig::FromSubscription("875f7907ac9c4036b76cb470b4b1ba6c", "eastus");
    // 设置语音名称，请参阅 https://aka.ms/speech/voices/neural 获取完整列表。
    config->SetSpeechSynthesisVoiceName("zh-CN-XiaoxiaoNeural");
    // 把文本的音频保存为文件，有多种AudioConfig格式，比如流
    auto audioOutput = AudioConfig::FromWavFileOutput("output.wav");
    // 使用音频保件作为音频输出，
    // auto synthesizer = SpeechSynthesizer::FromConfig(config, audioOutput);
    // 使用默认扬声器作为音频输出创建语音合成器。
    auto synthesizer = SpeechSynthesizer::FromConfig(config);
    std::string text = "LLMs can reason about wide-ranging topics, but their knowledge is limited to the public data up to a specific point in time that they were trained on.";
    auto result = synthesizer->SpeakTextAsync(text).get();
    //使用stream
    // auto stream = AudioDataStream::FromResult(result);
    // stream->SaveToWavFile("output.wav");
}

int main()
{
    try
    {
        synthesizeSpeech();
    }
    catch (exception e)
    {
        cout << e.what();
    }
    return 0;
}


