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

    // ── Sample list ──────────────────────────────────────────────────────────
    constexpr int listY     = 58;
    constexpr int rowH      = 18;
    constexpr int colGap    = 8;
    constexpr int maxChars  = 12;
    constexpr int btnSize   = 14;
    constexpr int btnGap    = 2;
    constexpr int btnsW     = btnSize * 4 + btnGap * 3; // 4 buttons

    const int colW = (getWidth() - 16 - colGap) / 2;
    const int col0X = 8;
    const int col1X = col0X + colW + colGap;
    const int nameW = colW - btnsW - 4;

    g.setFont(juce::Font(juce::FontOptions(11.0f)));

    rowHitAreas.clear();
    int loadedCount = 0;

    for (int i = 0; i < NewProjectAudioProcessor::NUM_SAMPLE_SLOTS; ++i)
    {
        if (!processor.sampleSlots[i].isLoaded)
            continue;

        int col = (loadedCount < 10) ? 0 : 1;
        int row = (loadedCount < 10) ? loadedCount : loadedCount - 10;
        int x   = (col == 0) ? col0X : col1X;
        int y   = listY + row * rowH;

        // Sample name
        juce::String slotNum = juce::String(i + 1).paddedLeft('0', 2);
        juce::String fileName = processor.sampleSlots[i].sourceFile.getFileName();
        juce::String display = slotNum + ". " + truncateName(fileName, maxChars);

        g.setColour(juce::Colour(0xffb0b0b0));
        g.setFont(juce::Font(juce::FontOptions(11.0f)));
        g.drawText(display, x, y, nameW, rowH, juce::Justification::centredLeft);

        // Action buttons: reorder, play, replace, delete
        int bx = x + nameW + 4;
        RowHitAreas hit;
        hit.slotIndex = i;

        // Reorder (arrow cross)
        hit.reorderBtn = { bx, y, btnSize, btnSize };
        g.setColour(juce::Colour(0xff707070));
        g.setFont(juce::Font(juce::FontOptions(11.0f)));
        g.drawText(juce::String::charToString(0x2725), hit.reorderBtn, juce::Justification::centred); // +
        bx += btnSize + btnGap;

        // Play (audition)
        hit.playBtn = { bx, y, btnSize, btnSize };
        g.setColour(juce::Colour(0xff80b080));
        g.drawText(juce::String::charToString(0x25B6), hit.playBtn, juce::Justification::centred); // play triangle
        bx += btnSize + btnGap;

        // Replace
        hit.replaceBtn = { bx, y, btnSize, btnSize };
        g.setColour(juce::Colour(0xff8080b0));
        g.drawText(juce::String::charToString(0x21C4), hit.replaceBtn, juce::Justification::centred); // swap arrows
        bx += btnSize + btnGap;

        // Delete
        hit.deleteBtn = { bx, y, btnSize, btnSize };
        g.setColour(juce::Colour(0xffb07070));
        g.drawText(juce::String::charToString(0x2715), hit.deleteBtn, juce::Justification::centred); // X

        rowHitAreas.push_back(hit);
        ++loadedCount;
    }

    // ── "Add more" text ──────────────────────────────────────────────────────
    int emptySlots = NewProjectAudioProcessor::NUM_SAMPLE_SLOTS - loadedCount;
    if (emptySlots > 0)
    {
        int addY;
        if (loadedCount == 0)
            addY = listY;
        else if (loadedCount <= 10)
            addY = listY + loadedCount * rowH + 4;
        else
            addY = listY + (loadedCount - 10) * rowH + 4;

        juce::String addText = "--- click to add " + juce::String(emptySlots) + " samples ---";
        g.setColour(juce::Colour(0xff606068));
        g.setFont(juce::Font(juce::FontOptions(10.0f)).italicised());
        g.drawText(addText, col0X, addY, getWidth() - 16, rowH, juce::Justification::centredLeft);

        addMoreArea = { col0X, addY, getWidth() - 16, rowH };
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
    playbackModeButton.setBounds(getWidth() - 98, headerY, 90, 26);
}

void SampleManagerPanel::mouseDown(const juce::MouseEvent& e)
{
    auto pos = e.getPosition();

    // Check action buttons
    for (auto& hit : rowHitAreas)
    {
        if (hit.playBtn.contains(pos))
        {
            if (onAuditionSample)
                onAuditionSample(hit.slotIndex);
            return;
        }
        if (hit.replaceBtn.contains(pos))
        {
            if (onReplaceSample)
                onReplaceSample(hit.slotIndex);
            return;
        }
        if (hit.deleteBtn.contains(pos))
        {
            deleteSample(hit.slotIndex);
            return;
        }
        // Reorder button — handled in Step 4.5 (drag-and-drop)
    }

    // Check "add more" area
    if (addMoreArea.contains(pos) && onAddMoreClicked)
        onAddMoreClicked();
}

void SampleManagerPanel::deleteSample(int slotIndex)
{
    processor.sampleLoader.clearSlot(slotIndex);

    // Shift samples above the deleted slot down to fill the gap
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
    repaint();
}
