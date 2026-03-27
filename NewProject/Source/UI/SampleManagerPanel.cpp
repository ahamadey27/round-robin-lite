#include "SampleManagerPanel.h"
#include "../PluginProcessor.h"

SampleManagerPanel::SampleManagerPanel(NewProjectAudioProcessor& p)
    : processor(p)
{
    loadSamplesButton.setButtonText("Load Samples");
    loadSamplesButton.onClick = [this]() { if (onLoadSamplesClicked) onLoadSamplesClicked(); };
    addAndMakeVisible(loadSamplesButton);

    playbackModeButton.setButtonText("Series");
    playbackModeButton.setClickingTogglesState(true);
    playbackModeButton.onClick = [this]()
        {
            bool isRandom = playbackModeButton.getToggleState();
            playbackModeButton.setButtonText(isRandom ? "Random" : "Series");
        };
    playbackModeButton.setLookAndFeel(&toggleLAF);
    addAndMakeVisible(playbackModeButton);

    playbackModeAttachment = std::make_unique<ButtonAttachment>(
        p.apvts, ParameterIDs::playbackMode, playbackModeButton);
}

SampleManagerPanel::~SampleManagerPanel()
{
    playbackModeButton.setLookAndFeel(nullptr);
}

juce::String SampleManagerPanel::truncateName(const juce::String& name, int maxChars)
{
    if (name.length() <= maxChars)
        return name;

    auto dot = name.lastIndexOfChar('.');
    juce::String ext = (dot >= 0) ? name.substring(dot) : "";
    int available = maxChars - ext.length() - 3;
    if (available < 1) available = 1;
    return name.substring(0, available) + "..." + ext;
}

void SampleManagerPanel::paint(juce::Graphics& g)
{
    // ── Section title ────────────────────────────────────────────────────────
    g.setColour(juce::Colour(0xff3a3a4a));
    g.setFont(juce::Font(juce::FontOptions(9.0f)).boldened());
    g.drawText("SAMPLE POOL", 8, 7, getWidth() - 16, 10, juce::Justification::left);

    // ── Playback type label above toggle ─────────────────────────────────────
    g.setColour(juce::Colour(0xff666666));
    g.setFont(juce::Font(juce::FontOptions(8.0f)));
    g.drawText("PLAYBACK TYPE", getWidth() - 168, 16, 160, 10, juce::Justification::centred);

    // ── Sample list (single column, 20 rows) ─────────────────────────────────
    constexpr int listY     = 58;
    constexpr int rowH      = 18;
    constexpr int btnSize   = 14;
    constexpr int btnGap    = 2;
    constexpr int btnsW     = btnSize * 4 + btnGap * 3;
    constexpr int padX      = 8;

    const int listW = getWidth() - padX * 2;
    const int nameW = listW - btnsW - 4;
    const int maxChars = juce::jmax(14, nameW / 7); // approximate chars that fit

    rowHitAreas.clear();
    int loadedCount = 0;

    for (int i = 0; i < NewProjectAudioProcessor::NUM_SAMPLE_SLOTS; ++i)
    {
        if (!processor.sampleSlots[i].isLoaded)
            continue;

        int y = listY + loadedCount * rowH;

        // Highlight drop target (swap)
        if (isDragging && !dragIsInsert && dragTargetSlot == i)
        {
            g.setColour(juce::Colour(0xff3a4a3a));
            g.fillRect(padX, y, listW, rowH);
        }

        // Sample name
        juce::String slotNum = juce::String(i + 1).paddedLeft('0', 2);
        juce::String fileName = processor.sampleSlots[i].sourceFile.getFileName();
        juce::String display = slotNum + ". " + truncateName(fileName, maxChars);

        g.setColour(isDragging && dragSourceSlot == i ? juce::Colour(0xff606060) : juce::Colour(0xffb0b0b0));
        g.setFont(juce::Font(juce::FontOptions(11.0f)));
        g.drawText(display, padX, y, nameW, rowH, juce::Justification::centredLeft);

        // Action buttons
        int bx = padX + nameW + 4;
        RowHitAreas hit;
        hit.slotIndex = i;
        hit.nameArea = { padX, y, nameW, rowH };
        hit.rowArea  = { padX, y, listW, rowH };

        hit.reorderBtn = { bx, y, btnSize, btnSize };
        g.setColour(juce::Colour(0xff707070));
        g.setFont(juce::Font(juce::FontOptions(11.0f)));
        g.drawText(juce::String::charToString(0x2725), hit.reorderBtn, juce::Justification::centred);
        bx += btnSize + btnGap;

        hit.playBtn = { bx, y, btnSize, btnSize };
        g.setColour(juce::Colour(0xff80b080));
        g.drawText(juce::String::charToString(0x25B6), hit.playBtn, juce::Justification::centred);
        bx += btnSize + btnGap;

        hit.replaceBtn = { bx, y, btnSize, btnSize };
        g.setColour(juce::Colour(0xff8080b0));
        g.drawText(juce::String::charToString(0x21C4), hit.replaceBtn, juce::Justification::centred);
        bx += btnSize + btnGap;

        hit.deleteBtn = { bx, y, btnSize, btnSize };
        g.setColour(juce::Colour(0xffb07070));
        g.drawText(juce::String::charToString(0x2715), hit.deleteBtn, juce::Justification::centred);

        rowHitAreas.push_back(hit);

        // Draw insertion line if dragging between rows
        if (isDragging && dragIsInsert && dragTargetSlot == i)
        {
            g.setColour(juce::Colour(0xff70c870));
            g.fillRect(padX, y - 1, listW, 2);
        }

        ++loadedCount;
    }

    // ── "Add more" text (always directly below last loaded sample) ───────────
    int emptySlots = NewProjectAudioProcessor::NUM_SAMPLE_SLOTS - loadedCount;
    if (emptySlots > 0)
    {
        int addY = listY + loadedCount * rowH + 4;

        juce::String addText = "--- click to add " + juce::String(emptySlots) + " samples ---";
        g.setColour(juce::Colour(0xff606068));
        g.setFont(juce::Font(juce::FontOptions(10.0f)).italicised());
        g.drawText(addText, padX, addY, listW, rowH, juce::Justification::centredLeft);

        addMoreArea = { padX, addY, listW, rowH };
    }
    else
    {
        addMoreArea = {};
    }
}

void SampleManagerPanel::resized()
{
    constexpr int headerY = 24;
    loadSamplesButton .setBounds(8, headerY, 120, 26);
    // Horizontal toggle: needs ~160px wide for labels + pill, 20px tall
    playbackModeButton.setBounds(getWidth() - 168, headerY + 3, 160, 20);
}

int SampleManagerPanel::getSlotAtPosition(juce::Point<int> pos, bool& isInsertGap) const
{
    isInsertGap = false;

    for (auto& hit : rowHitAreas)
    {
        if (hit.rowArea.contains(pos))
        {
            int relY = pos.y - hit.rowArea.getY();
            if (relY < 4)
                isInsertGap = true;
            return hit.slotIndex;
        }
    }
    return -1;
}

void SampleManagerPanel::mouseDown(const juce::MouseEvent& e)
{
    auto pos = e.getPosition();

    for (auto& hit : rowHitAreas)
    {
        if (hit.reorderBtn.contains(pos))
        {
            isDragging = true;
            dragSourceSlot = hit.slotIndex;
            dragTargetSlot = -1;
            dragIsInsert = false;
            dragPos = pos;
            return;
        }
        if (hit.playBtn.contains(pos))
        {
            if (onAuditionSample) onAuditionSample(hit.slotIndex);
            return;
        }
        if (hit.replaceBtn.contains(pos))
        {
            if (onReplaceSample) onReplaceSample(hit.slotIndex);
            return;
        }
        if (hit.deleteBtn.contains(pos))
        {
            deleteSample(hit.slotIndex);
            return;
        }
    }

    if (addMoreArea.contains(pos) && onAddMoreClicked)
        onAddMoreClicked();
}

void SampleManagerPanel::mouseDrag(const juce::MouseEvent& e)
{
    if (!isDragging) return;

    dragPos = e.getPosition();
    bool isInsert = false;
    int target = getSlotAtPosition(dragPos, isInsert);

    if (target == dragSourceSlot)
        target = -1;

    dragTargetSlot = target;
    dragIsInsert = isInsert;
    repaint();
}

void SampleManagerPanel::mouseUp(const juce::MouseEvent& e)
{
    if (!isDragging) return;

    if (dragTargetSlot >= 0 && dragSourceSlot >= 0 && dragTargetSlot != dragSourceSlot)
    {
        if (dragIsInsert)
            processor.insertSample(dragSourceSlot, dragTargetSlot);
        else
            processor.swapSamples(dragSourceSlot, dragTargetSlot);
    }

    isDragging = false;
    dragSourceSlot = -1;
    dragTargetSlot = -1;
    repaint();
}

void SampleManagerPanel::mouseMove(const juce::MouseEvent& e)
{
    auto pos = e.getPosition();
    juce::String newTooltip;

    for (auto& hit : rowHitAreas)
    {
        if (hit.nameArea.contains(pos))
        {
            // Show full filename as tooltip if it was truncated
            juce::String fullName = processor.sampleSlots[hit.slotIndex].sourceFile.getFileName();
            int maxChars = juce::jmax(14, hit.nameArea.getWidth() / 7);
            if (fullName.length() > maxChars)
                newTooltip = fullName;
            break;
        }
    }

    if (newTooltip != currentTooltip)
    {
        currentTooltip = newTooltip;
        // TooltipWindow picks up changes via getTooltip() automatically
    }
}

void SampleManagerPanel::mouseExit(const juce::MouseEvent&)
{
    currentTooltip = {};
}

void SampleManagerPanel::deleteSample(int slotIndex)
{
    processor.sampleLoader.clearSlot(slotIndex);

    for (int i = slotIndex; i < NewProjectAudioProcessor::NUM_SAMPLE_SLOTS - 1; ++i)
    {
        if (processor.sampleSlots[i + 1].isLoaded)
        {
            processor.sampleSlots[i] = processor.sampleSlots[i + 1];
            processor.sampleSlots[i + 1].clear();
        }
        else
        {
            break;
        }
    }

    processor.rebuildLoadedIndices();
    processor.reshuffleIndices();
    processor.sampleLoader.updateSynthesiserSounds();
    repaint();
}
