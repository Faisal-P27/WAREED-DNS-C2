#include "Include/helper/colortext.hpp"


ColorText::ColorText()
{

}


QString ColorText::Colors::Hex::Background    = "#282a36";
QString ColorText::Colors::Hex::Foreground    = "#f8f8f2";
QString ColorText::Colors::Hex::Comment       = "#6272a4";
QString ColorText::Colors::Hex::CurrentLine   = "#44475a";

QString ColorText::Colors::Hex::Cyan          = "#8be9fd";
QString ColorText::Colors::Hex::Green         = "#50fa7b";
QString ColorText::Colors::Hex::Orange        = "#ffb86c";
QString ColorText::Colors::Hex::Pink          = "#ff79c6";
QString ColorText::Colors::Hex::Purple        = "#bd93f9";
QString ColorText::Colors::Hex::Red           = "#ff5555";
QString ColorText::Colors::Hex::Yellow        = "#f1fa8c";

void ColorText::SetDraculaDark()
{
    ColorText::Colors::Hex::Background    = "#282a36";
    ColorText::Colors::Hex::Foreground    = "#f8f8f2";
    ColorText::Colors::Hex::Comment       = "#6272a4";
    ColorText::Colors::Hex::CurrentLine   = "#44475a";

    ColorText::Colors::Hex::Cyan          = "#8be9fd";
    ColorText::Colors::Hex::Green         = "#50fa7b";
    ColorText::Colors::Hex::Orange        = "#ffb86c";
    ColorText::Colors::Hex::Pink          = "#ff79c6";
    ColorText::Colors::Hex::Purple        = "#bd93f9";
    ColorText::Colors::Hex::Red           = "#ff5555";
    ColorText::Colors::Hex::Yellow        = "#f1fa8c";
}

void ColorText::SetDraculaLight()
{
    // TODO: get white theme
}

QString ColorText::Color(const QString& color, const QString &text)
{
    return "<span style=\"color: "+ color +";\" >" + text.toHtmlEscaped() + "</span>";
}

QString ColorText::Background(const QString& text)
{
    return "<span style=\"color: "+ ColorText::Colors::Hex::Background +";\" >" + text.toHtmlEscaped() + "</span>";
}

QString ColorText::Foreground(const QString& text) {
    return "<span style=\"color: "+ ColorText::Colors::Hex::Foreground +";\" >" + text.toHtmlEscaped() + "</span>";
}

QString ColorText::Comment(const QString& text) {
    return "<span style=\"color: "+ ColorText::Colors::Hex::Comment +";\" >" + text.toHtmlEscaped() + "</span>";
}

QString ColorText::Cyan(const QString& text) {
    return "<span style=\"color: "+ ColorText::Colors::Hex::Cyan +";\" >" + text.toHtmlEscaped() + "</span>";
}

QString ColorText::Green(const QString& text) {
    return "<span style=\"color: "+ ColorText::Colors::Hex::Green +";\" >" + text.toHtmlEscaped() + "</span>";
}

QString ColorText::Orange(const QString& text) {
    return "<span style=\"color: "+ ColorText::Colors::Hex::Orange +";\" >" + text.toHtmlEscaped() + "</span>";
}

QString ColorText::Pink(const QString& text) {
    return "<span style=\"color: "+ ColorText::Colors::Hex::Pink +";\" >" + text.toHtmlEscaped() + "</span>";
}

QString ColorText::Purple(const QString& text) {
    return "<span style=\"color: "+ ColorText::Colors::Hex::Purple +";\" >" + text.toHtmlEscaped() + "</span>";
}

QString ColorText::Red(const QString& text) {
    return "<span style=\"color: "+ ColorText::Colors::Hex::Red +";\" >" + text.toHtmlEscaped() + "</span>";
}

QString ColorText::Yellow(const QString& text) {
    return "<span style=\"color: "+ ColorText::Colors::Hex::Yellow +";\" >" + text.toHtmlEscaped() + "</span>";
}

QString ColorText::Bold(const QString& text) {
    return "<b>" + text.toHtmlEscaped() + "</b>";
}

QString ColorText::Underline(const QString &text) {
    return "<span style=\"text-decoration:underline\">" + text.toHtmlEscaped() + "</span>";
}

QString ColorText::UnderlineBackground(const QString &text) {
    return "<span style=\"text-decoration:underline; color: "+ ColorText::Colors::Hex::Background +";\" >" + text.toHtmlEscaped() + "</span>";
}

QString ColorText::UnderlineForeground(const QString &text) {
    return "<span style=\"text-decoration:underline; color: "+ ColorText::Colors::Hex::Foreground +";\" >" + text.toHtmlEscaped() + "</span>";
}

QString ColorText::UnderlineComment(const QString &text) {
    return "<span style=\"text-decoration:underline; color: "+ ColorText::Colors::Hex::Comment +";\" >" + text.toHtmlEscaped() + "</span>";
}

QString ColorText::UnderlineCyan(const QString &text) {
    return "<span style=\"text-decoration:underline; color: "+ ColorText::Colors::Hex::Cyan +";\" >" + text.toHtmlEscaped() + "</span>";
}

QString ColorText::UnderlineGreen(const QString &text) {
    return "<span style=\"text-decoration:underline; color: "+ ColorText::Colors::Hex::Green +";\" >" + text.toHtmlEscaped() + "</span>";
}

QString ColorText::UnderlineOrange(const QString &text) {
    return "<span style=\"text-decoration:underline; color: "+ ColorText::Colors::Hex::Orange +";\" >" + text.toHtmlEscaped() + "</span>";
}

QString ColorText::UnderlinePink(const QString &text) {
    return "<span style=\"text-decoration:underline; color: "+ ColorText::Colors::Hex::Pink +";\" >" + text.toHtmlEscaped() + "</span>";
}

QString ColorText::UnderlinePurple(const QString &text) {
    return "<span style=\"text-decoration:underline; color: "+ ColorText::Colors::Hex::Purple +";\" >" + text.toHtmlEscaped() + "</span>";
}

QString ColorText::UnderlineRed(const QString &text) {
    return "<span style=\"text-decoration:underline; color: "+ ColorText::Colors::Hex::Red +";\" >" + text.toHtmlEscaped() + "</span>";
}

QString ColorText::UnderlineYellow(const QString &text) {
    return "<span style=\"text-decoration:underline; color: "+ ColorText::Colors::Hex::Yellow +";\" >" + text.toHtmlEscaped() + "</span>";
}
