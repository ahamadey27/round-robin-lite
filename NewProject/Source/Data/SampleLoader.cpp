#include "SampleLoader.h"

SampleLoader::SampleLoader(juce::AudioFormatManager& formatManager,
    juce::Synthesiser& synthesiser,
    SampleSlot* slots,
    int          numSlots)
    : formatManager(formatManager),
    synthesiser(synthesiser),
    slots(slots),
    numSlots(numSlots)
{
}

bool SampleLoader::loadSample(int slotIndex, const juce::File& file)
{
    if (slotIndex < 0 || slotIndex >= numSlots)
    {
        DBG("SampleLoader: Invalid slot index: " + juce::String(slotIndex));
        return false;
    }

    // --- Error Check 1: File existence ---
    if (!file.existsAsFile())
    {
        DBG("SampleLoader: File not found: " + file.getFullPathName());
        lastErrorMessage = "File not found: " + file.getFileName();
        return false;
    }

    // --- Error Check 2: File size limit (200MB) ---
    const int64_t maxFileSizeBytes = 200LL * 1024 * 1024;
    if (file.getSize() > maxFileSizeBytes)
    {
        DBG("SampleLoader: File too large (" + juce::String(file.getSize() / (1024 * 1024)) + "MB): " + file.getFileName());
        lastErrorMessage = "File too large (max 200MB): " + file.getFileName();
        return false;
    }

    // --- Error Check 3: Supported format ---
    auto extension = file.getFileExtension().toLowerCase();
    juce::StringArray supportedFormats = { ".wav", ".aif", ".aiff", ".flac", ".ogg" };
    if (!supportedFormats.contains(extension))
    {
        DBG("SampleLoader: Unsupported format '" + extension + "': " + file.getFileName());
        lastErrorMessage = "Unsupported format '" + extension + "'. Use WAV, AIFF, FLAC, or OGG.";
        return false;
    }

    // --- Error Check 4: Readable by format manager (catches corrupted files) ---
    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(file));
    if (reader == nullptr)
    {
        DBG("SampleLoader: Could not read file (corrupted or truly unsupported): " + file.getFileName());
        lastErrorMessage = "Could not read file - it may be corrupted: " + file.getFileName();
        return false;
    }

    // --- Error Check 5: Unsupported sample rate ---
    const double minSampleRate = 8000.0;
    const double maxSampleRate = 192000.0;
    if (reader->sampleRate < minSampleRate || reader->sampleRate > maxSampleRate)
    {
        DBG("SampleLoader: Unsupported sample rate (" + juce::String(reader->sampleRate) + "Hz): " + file.getFileName());
        lastErrorMessage = "Unsupported sample rate (" + juce::String((int)reader->sampleRate) + "Hz): " + file.getFileName();
        return false;
    }

    // --- Attempt load into slot ---
    // Reader ownership passes back to loadFromFile, so release here
    // Note: loadFromFile will create its own reader internally
    reader.reset();

    bool success = slots[slotIndex].loadFromFile(file, formatManager);

    if (success)
    {
        // --- Error Check 6: Verify buffer was actually populated ---
        if (slots[slotIndex].audioBuffer.getNumSamples() == 0 ||
            slots[slotIndex].audioBuffer.getNumChannels() == 0)
        {
            DBG("SampleLoader: Buffer empty after load - possible memory issue: " + file.getFileName());
            lastErrorMessage = "Failed to allocate audio buffer for: " + file.getFileName();
            slots[slotIndex].clear();
            return false;
        }

        lastErrorMessage.clear();
        DBG("Slot " + juce::String(slotIndex) + " loaded: " + slots[slotIndex].displayName);
        updateSynthesiserSounds();
    }
    else
    {
        // loadFromFile failed - fallback: ensure slot is clean
        slots[slotIndex].clear();
        lastErrorMessage = "Failed to load: " + file.getFileName();
        DBG("Slot " + juce::String(slotIndex) + " failed to load: " + file.getFullPathName());
    }

    return success;
}

void SampleLoader::clearSlot(int slotIndex)
{
    if (slotIndex < 0 || slotIndex >= numSlots)
        return;

    slots[slotIndex].clear();
    lastErrorMessage.clear();
    DBG("Slot " + juce::String(slotIndex) + " cleared");
    updateSynthesiserSounds();
}

void SampleLoader::updateSynthesiserSounds()
{
    synthesiser.clearSounds();

    int loadedCount = 0;
    for (int i = 0; i < numSlots; ++i)
    {
        if (!slots[i].isLoaded)
            continue;

        auto* sound = new RRSound();
        sound->setFromSlot(slots[i]);
        synthesiser.addSound(sound);
        ++loadedCount;
    }

    DBG("Synthesiser updated: " + juce::String(loadedCount) + " sound(s) loaded");
}