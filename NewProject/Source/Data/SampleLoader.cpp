#include "SampleLoader.h"

SampleLoader::SampleLoader(juce::AudioFormatManager& formatManager,
    juce::Synthesiser& synthesiser,
    SampleSlot* slots,
    int                       numSlots)
    : formatManager(formatManager),
    synthesiser(synthesiser),
    slots(slots),
    numSlots(numSlots)
{
}

bool SampleLoader::loadSample(int slotIndex, const juce::File& file)
{
    if (slotIndex < 0 || slotIndex >= numSlots)
        return false;

    bool success = slots[slotIndex].loadFromFile(file, formatManager);

    if (success)
    {
        DBG("Slot " + juce::String(slotIndex) + " loaded: " + slots[slotIndex].displayName);
        updateSynthesiserSounds();
    }
    else
    {
        DBG("Slot " + juce::String(slotIndex) + " failed to load: " + file.getFullPathName());
    }

    return success;
}

void SampleLoader::clearSlot(int slotIndex)
{
    if (slotIndex < 0 || slotIndex >= numSlots)
        return;

    slots[slotIndex].clear();
    DBG("Slot " + juce::String(slotIndex) + " cleared");
    updateSynthesiserSounds();
}

void SampleLoader::updateSynthesiserSounds()
{
    // Remove all existing sounds
    synthesiser.clearSounds();

    int loadedCount = 0;

    for (int i = 0; i < numSlots; ++i)
    {
        if (!slots[i].isLoaded)
            continue;

        auto* sound = new RRSound();

        // Copy audio buffer from slot into the RRSound
        sound->setFromSlot(slots[i]);

        synthesiser.addSound(sound);
        ++loadedCount;
    }

    DBG("Synthesiser updated: " + juce::String(loadedCount) + " sound(s) loaded");
}