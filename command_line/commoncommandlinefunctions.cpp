//Copyright 2015 Ryan Wick

//This file is part of Bandage

//Bandage is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//Bandage is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with Bandage.  If not, see <http://www.gnu.org/licenses/>.


#include "commoncommandlinefunctions.h"
#include "../graph/assemblygraph.h"
#include <ogdf/energybased/FMMMLayout.h>
#include "../program/graphlayoutworker.h"
#include "../program/settings.h"

bool loadAssemblyGraph(QString filename)
{
    g_assemblyGraph = new AssemblyGraph();
    return g_assemblyGraph->loadGraphFromFile(filename);
}



//Unlike the equivalent function in MainWindow, this does the graph layout in the main thread.
void layoutGraph()
{
    ogdf::FMMMLayout fmmm;
    GraphLayoutWorker * graphLayoutWorker = new GraphLayoutWorker(&fmmm, g_assemblyGraph->m_graphAttributes,
                                                                  g_settings->graphLayoutQuality, g_settings->segmentLength);
    graphLayoutWorker->layoutGraph();
}

bool checkForHelp(QStringList arguments)
{
    int h1 = arguments.indexOf("-h");
    int h2 = arguments.indexOf("-help");
    int h3 = arguments.indexOf("--help");

    return (h1 != -1 || h2 != -1 || h3 != -1);
}

bool checkForHelpAll(QStringList arguments)
{
    return (arguments.indexOf("--helpall") != -1);
}

bool checkForVersion(QStringList arguments)
{
    int v1 = arguments.indexOf("-v");
    int v2 = arguments.indexOf("-version");
    int v3 = arguments.indexOf("--version");

    return (v1 != -1 || v2 != -1 || v3 != -1);
}




//Returns empty string if everything is okay and an error
//message if there's a problem.  If everything is okay, it
//also removes the option and its value from arguments.
QString checkOptionForInt(QString option, QStringList * arguments, int min, int max)
{
    int optionIndex = arguments->indexOf(option);

    //If the option isn't found, that's fine.
    if (optionIndex == -1)
        return "";

    int integerIndex = optionIndex + 1;

    //If nothing follows the option, that's a problem.
    if (integerIndex >= arguments->size())
        return option + " must be followed by an integer";

    //If the thing following the option isn't an integer, that's a problem.
    bool optionIsInt;
    int optionInt = arguments->at(integerIndex).toInt(&optionIsInt);
    if (!optionIsInt)
        return option + " must be followed by an integer";

    //Check the range of the option.
    if (optionInt < min || optionInt > max)
        return "Value of " + option + " must be between "
                + QString::number(min) + " and " + QString::number(max) +
                " (inclusive)";

    //If the code got here, the option and its integer are okay.
    //Remove them from the arguments.
    arguments->removeAt(integerIndex);
    arguments->removeAt(optionIndex);

    return "";
}


//Returns empty string if everything is okay and an error
//message if there's a problem.  If everything is okay, it
//also removes the option and its value from arguments.
QString checkOptionForFloat(QString option, QStringList * arguments, double min, double max)
{
    int optionIndex = arguments->indexOf(option);

    //If the option isn't found, that's fine.
    if (optionIndex == -1)
        return "";

    int floatIndex = optionIndex + 1;

    //If nothing follows the option, that's a problem.
    if (floatIndex >= arguments->size())
        return option + " must be followed by a number";

    //If the thing following the option isn't a number, that's a problem.
    bool optionIsFloat;
    double optionFloat = arguments->at(floatIndex).toDouble(&optionIsFloat);
    if (!optionIsFloat)
        return option + " must be followed by a number";

    //Check the range of the option.
    if (optionFloat < min || optionFloat > max)
        return "Value of " + option + " must be between "
                + QString::number(min) + " and " + QString::number(max) +
                " (inclusive)";

    //If the code got here, the option and its number are okay.
    //Remove them from the arguments.
    arguments->removeAt(floatIndex);
    arguments->removeAt(optionIndex);

    return "";
}


//Returns empty string if everything is okay and an error
//message if there's a problem.  If everything is okay, it
//also removes the option and its value from arguments.
QString checkOptionForString(QString option, QStringList * arguments, QStringList validOptionsList)
{
    int optionIndex = arguments->indexOf(option);

    //If the option isn't found, that's fine.
    if (optionIndex == -1)
        return "";

    int stringIndex = optionIndex + 1;

    QString validOptions;
    for (int i = 0; i < validOptionsList.size(); ++i)
    {
        validOptions += validOptionsList.at(i);
        if (i == validOptionsList.size() - 2)
            validOptions += " or ";
        else if (i < validOptionsList.size() - 2)
            validOptions += ", ";
    }

    //If nothing follows the option, that's a problem.
    if (stringIndex >= arguments->size())
        return option + " must be followed by " + validOptions;

    //If the thing following the option isn't a valid choice, that's a problem.
    QString value = arguments->at(stringIndex);
    if (!validOptionsList.contains(value, Qt::CaseInsensitive))
        return option + " must be followed by " + validOptions;

    //If the code got here, the option and its string are okay.
    //Remove them from the arguments.
    arguments->removeAt(stringIndex);
    arguments->removeAt(optionIndex);

    return "";
}


QString checkOptionForHexColour(QString option, QStringList * arguments)
{
    int optionIndex = arguments->indexOf(option);

    //If the option isn't found, that's fine.
    if (optionIndex == -1)
        return "";

    int hexIndex = optionIndex + 1;

    //If nothing follows the option, that's a problem.
    if (hexIndex >= arguments->size())
        return option + " must be followed by a 6 character HTML-style hex colour";

    //If the thing following the option isn't a hex colour, that's a problem.
    QString hexName = "#" + arguments->at(hexIndex);
    QColor colour(hexName);
    if (!colour.isValid())
        return option + " must be followed by a 6 character HTML-style hex colour";

    //If the code got here, the option and its hex colour are okay.
    //Remove them from the arguments.
    arguments->removeAt(hexIndex);
    arguments->removeAt(optionIndex);

    return "";
}


//This function simply removes an option from arguments if it is found.
void checkOptionWithoutValue(QString option, QStringList * arguments)
{
    int optionIndex = arguments->indexOf(option);

    //If the option isn't found, that's fine.
    if (optionIndex == -1)
        return ;

    //If the code got here, the option is okay.
    //Remove it from the arguments.
    arguments->removeAt(optionIndex);
}


//This function checks to make sure either both or neither of the options
//are used.  It can also optionally check to make sure the second is larger
//than the first.
QString checkTwoOptionsForFloats(QString option1, QString option2, QStringList * arguments,
                                 double min1, double max1, double min2, double max2,
                                 bool secondMustBeEqualOrLarger)
{
    //First check each option independently
    QStringList argumentsCopy = *arguments;
    QString option1Error = checkOptionForFloat(option1, &argumentsCopy, min1, max1);
    if (option1Error != "")
        return option1Error;
    QString option2Error = checkOptionForFloat(option2, &argumentsCopy, min2, max2);
    if (option2Error != "")
        return option2Error;

    //Now make sure either both or neither are present.
    if (isOptionPresent(option1, arguments) != isOptionPresent(option2, arguments))
        return option1 + " and " + option2 + " must be used together";

    if (secondMustBeEqualOrLarger)
    {
        if (getFloatOption(option2, arguments) < getFloatOption(option1, arguments))
            return option2 + " must be greater than or equal to " + option1;
    }

    //Now remove the options from the arguments before finishing.
    checkOptionForFloat(option1, arguments, min1, max1);
    checkOptionForFloat(option2, arguments, min2, max2);
    return "";
}



bool isOptionPresent(QString option, QStringList * arguments)
{
    return (arguments->indexOf(option) > -1);
}



int getIntOption(QString option, QStringList * arguments)
{
    int optionIndex = arguments->indexOf(option);
    if (optionIndex == -1)
        return 0;

    int integerIndex = optionIndex + 1;
    if (integerIndex >= arguments->size())
        return 0;

    return arguments->at(integerIndex).toInt();
}

double getFloatOption(QString option, QStringList * arguments)
{
     int optionIndex = arguments->indexOf(option);
     if (optionIndex == -1)
         return 0;

     int floatIndex = optionIndex + 1;
     if (floatIndex >= arguments->size())
         return 0;

     return arguments->at(floatIndex).toDouble();
}

NodeColourScheme getColourSchemeOption(QString option, QStringList * arguments)
{
    int optionIndex = arguments->indexOf(option);
    if (optionIndex == -1)
        return RANDOM_COLOURS;

    int colourIndex = optionIndex + 1;
    if (colourIndex >= arguments->size())
        return RANDOM_COLOURS;

    QString colourString = arguments->at(colourIndex).toLower();
    if (colourString == "random")
        return RANDOM_COLOURS;
    else if (colourString == "uniform")
        return ONE_COLOUR;
    else if (colourString == "coverage")
        return COVERAGE_COLOUR;

    //Random colours is the default
    return RANDOM_COLOURS;
}


QColor getHexColourOption(QString option, QStringList * arguments)
{
    int optionIndex = arguments->indexOf(option);
    if (optionIndex == -1)
        return QColor();

    int hexIndex = optionIndex + 1;
    if (hexIndex >= arguments->size())
        return QColor();

    return QColor("#" + arguments->at(hexIndex));
}



//This function checks the values of the Bandage settings.
//If everything is fine, it removes the good arguments/values and returns
//a null string.  If there's a problem, it returns an error message.
QString checkForInvalidOrExcessSettings(QStringList * arguments)
{
    checkOptionWithoutValue("--double", arguments);
    QString error = checkOptionForInt("--bases", arguments, 1, std::numeric_limits<int>::max());
    if (error.length() > 0) return error;
    error = checkOptionForInt("--quality", arguments, 1, 5);
    if (error.length() > 0) return error;

    error = checkOptionForFloat("--nodewidth", arguments, 0.5, 1000);
    if (error.length() > 0) return error;
    error = checkOptionForFloat("--covwidth", arguments, 0.0, 1.0);
    if (error.length() > 0) return error;
    error = checkOptionForFloat("--covpower", arguments, 0.1, 1.0);
    if (error.length() > 0) return error;

    error = checkOptionForFloat("--edgewidth", arguments, 0.1, 1000.0);
    if (error.length() > 0) return error;
    error = checkOptionForFloat("--outline", arguments, 0.0, 1000.0);
    if (error.length() > 0) return error;

    checkOptionWithoutValue("--names", arguments);
    checkOptionWithoutValue("--lengths", arguments);
    checkOptionWithoutValue("--coverages", arguments);
    error = checkOptionForInt("--fontsize", arguments, 1, 100);
    if (error.length() > 0) return error;
    error = checkOptionForFloat("--toutline", arguments, 0.0, 2.0);
    if (error.length() > 0) return error;

    QStringList validColourOptions;
    validColourOptions << "random" << "uniform" << "coverage";
    error = checkOptionForString("--colour", arguments, validColourOptions);
    if (error.length() > 0) return error;

    error = checkOptionForHexColour("--unicolpos", arguments);
    if (error.length() > 0) return error;
    error = checkOptionForHexColour("--unicolneg", arguments);
    if (error.length() > 0) return error;
    error = checkOptionForHexColour("--unicolspe", arguments);
    if (error.length() > 0) return error;

    error = checkOptionForHexColour("--covcollow", arguments);
    if (error.length() > 0) return error;
    error = checkOptionForHexColour("--covcolhi", arguments);
    if (error.length() > 0) return error;

    error = checkTwoOptionsForFloats("--covvallow", "--covvalhi", arguments, 0.0, 1000000.0, 0.0, 1000000.0, true);
    if (error.length() > 0) return error;

    return checkForExcessArguments(*arguments);
}



//This function generates an error if excess arguments are left after
//parsing.
QString checkForExcessArguments(QStringList arguments)
{
    if (arguments.size() == 0)
        return "";

    QString invalidOptionText = "Invalid option";
    if (arguments.size() > 1)
        invalidOptionText += "s";
    invalidOptionText += ": ";
    for (int i = 0; i < arguments.size(); ++i)
    {
        invalidOptionText += arguments.at(i);
        if (i < arguments.size() - 1)
            invalidOptionText += ", ";
    }

    return invalidOptionText;
}


void printCommonHelp(QTextStream * out)
{
    *out << "          --help              view this help message" << endl;
    *out << "          --helpall           view all command line settings" << endl;
    *out << endl;
//           ------------------------------|------------------------------------------------|  //80 character guide
}

void printSettingsUsage(QTextStream * out)
{
    *out << "Settings: --double            draw graph in double mode (default: off)" << endl;
    *out << "          --bases <int>       base pairs per segment (default: auto)" << endl;
    *out << "                              High values result in longer nodes, small values" << endl;
    *out << "                              in shorter nodes." << endl;
    *out << "          --quality <int>     graph layout quality, 1 (low) to 5 (high)" << endl;
    *out << "                              (default: 3)" << endl;
    *out << endl;
//           ------------------------------|------------------------------------------------|  //80 character guide
    *out << "          --nodewidth <float> Average node width (0.5 to 1000, default: 5.0)" << endl;
    *out << "          --covwidth <float>  Coverage effect on width (0 to 1, default: 0.5)" << endl;
    *out << "          --covpower <float>  Power of coverage effect on width (0.1 to 1," << endl;
    *out << "                              default: 0.5)" << endl;
    *out << "                              Node widths are determined using the following" << endl;
    *out << "                              formula: a*b*((c/d)^e-1)+1" << endl;
    *out << "                                       a = average node width" << endl;
    *out << "                                       b = coverage effect on width" << endl;
    *out << "                                       c = node coverage" << endl;
    *out << "                                       d = mean coverage" << endl;
    *out << "                                       e = power of coverage effect on width" << endl;
    *out << endl;
//           ------------------------------|------------------------------------------------|  //80 character guide
    *out << "          --edgewidth <float> Edge width (0.1 to 1000, default: 2.0)" << endl;
    *out << "          --outline <float>   Node outline thickness (0 to 1000, default: 0.5) " << endl;
    *out << endl;
//           ------------------------------|------------------------------------------------|  //80 character guide
    *out << "          --names             Label nodes with name" << endl;
    *out << "          --lengths           Label nodes with length" << endl;
    *out << "          --coverages         Label nodes with coverage" << endl;
    *out << "          --fontsize <int>    Font size for node labels (1 to 100, default: 10)" << endl;
    *out << "          --toutline <float>  Surround text with a white outline with this" << endl;
    *out << "                              thickness (default: 0.3))" << endl;
    *out << endl;
//           ------------------------------|------------------------------------------------|  //80 character guide
    *out << "          --colour <scheme>   Node colouring scheme, choose one of the" << endl;
    *out << "                              following options: random, uniform or coverage" << endl;
    *out << "                              (default: random)" << endl;
    *out << "                              Specific colours can be set using HTML-style" << endl;
    *out << "                              hexidemical values." << endl;
    *out << endl;
//           ------------------------------|------------------------------------------------|  //80 character guide
    *out << "          --unicolpos <hex>   Colour for positive nodes under the uniform" << endl;
    *out << "                              colouring scheme (default: " + g_settings->uniformPositiveNodeColour.name().right(6) + ")" << endl;
    *out << "          --unicolneg <hex>   Colour for negative nodes under the uniform" << endl;
    *out << "                              colouring scheme (default: " + g_settings->uniformNegativeNodeColour.name().right(6) + ")" << endl;
    *out << "          --unicolspe <hex>   Colour for special nodes under the uniform" << endl;
    *out << "                              colouring scheme (default: " + g_settings->uniformNodeSpecialColour.name().right(6) + ")" << endl;
    *out << endl;
//           ------------------------------|------------------------------------------------|  //80 character guide
    *out << "          --covcollow <hex>   Colour for nodes with coverage below the low" << endl;
    *out << "                              coverage colour value (default: " + g_settings->lowCoverageColour.name().right(6) + ")" << endl;
    *out << "          --covcolhi <hex>    Colour for nodes with coverage above the high" << endl;
    *out << "                              coverage colour value (default: " + g_settings->highCoverageColour.name().right(6) + ")" << endl;
    *out << "          --covvallow <float> Low coverage colour value (default: auto)" << endl;
    *out << "          --covvalhi <float>  High coverage colour value (default: auto)" << endl;
    *out << endl;
}


void parseSettings(QStringList arguments)
{
    g_settings->doubleMode = isOptionPresent("--double", &arguments);

    if (isOptionPresent("--bases", &arguments))
    {
        g_settings->manualBasePairsPerSegment = getIntOption("--bases", &arguments);
        g_settings->nodeLengthMode = MANUAL_NODE_LENGTH;
    }

    if (isOptionPresent("--quality", &arguments))
    {
        int quality = getIntOption("--quality", &arguments);
        if (quality < 0)
            quality = 0;
        if (quality > 4)
            quality = 4;
        g_settings->graphLayoutQuality = quality;
    }

    if (isOptionPresent("--nodewidth", &arguments))
        g_settings->averageNodeWidth = getFloatOption("--nodewidth", &arguments);

    if (isOptionPresent("--covwidth", &arguments))
        g_settings->coverageEffectOnWidth = getFloatOption("--covwidth", &arguments);

    if (isOptionPresent("--covpower", &arguments))
        g_settings->coveragePower = getFloatOption("--covpower", &arguments);

    if (isOptionPresent("--edgewidth", &arguments))
        g_settings->edgeWidth = getFloatOption("--edgewidth", &arguments);

    if (isOptionPresent("--outline", &arguments))
        g_settings->outlineThickness = getFloatOption("--outline", &arguments);
    else
        g_settings->outlineThickness = 0.5;

    g_settings->displayNodeNames = isOptionPresent("--names", &arguments);

    g_settings->displayNodeLengths = isOptionPresent("--lengths", &arguments);

    g_settings->displayNodeCoverages = isOptionPresent("--coverages", &arguments);

    QFont font = g_settings->labelFont;
    int fontsize = 10;
    if (isOptionPresent("--fontsize", &arguments))
        fontsize = getIntOption("--fontsize", &arguments);
    font.setPointSize(fontsize);
    g_settings->labelFont = font;

    if (isOptionPresent("--toutline", &arguments))
    {
        double textOutlineThickness = getFloatOption("--toutline", &arguments);
        if (textOutlineThickness == 0.0)
            g_settings->textOutline = false;
        else
        {
            g_settings->textOutline = true;
            g_settings->textOutlineThickness = textOutlineThickness;
        }
    }
    else
    {
        g_settings->textOutline = true;
        g_settings->textOutlineThickness = 0.3;
    }

    if (isOptionPresent("--colour", &arguments))
        g_settings->nodeColourScheme = getColourSchemeOption("--colour", &arguments);

    if (isOptionPresent("--unicolpos", &arguments))
        g_settings->uniformPositiveNodeColour = getHexColourOption("--unicolpos", &arguments);

    if (isOptionPresent("--unicolneg", &arguments))
        g_settings->uniformNegativeNodeColour = getHexColourOption("--unicolneg", &arguments);

    if (isOptionPresent("--unicolspe", &arguments))
        g_settings->uniformNodeSpecialColour = getHexColourOption("--unicolspe", &arguments);

    if (isOptionPresent("--covcollow", &arguments))
        g_settings->lowCoverageColour = getHexColourOption("--covcollow", &arguments);

    if (isOptionPresent("--covcolhi", &arguments))
        g_settings->highCoverageColour = getHexColourOption("--covcolhi", &arguments);

    if (isOptionPresent("--covvallow", &arguments))
    {
        g_settings->lowCoverageValue = getFloatOption("--covvallow", &arguments);
        g_settings->autoCoverageValue = false;
    }

    if (isOptionPresent("--covvalhi", &arguments))
    {
        g_settings->highCoverageValue = getFloatOption("--covvalhi", &arguments);
        g_settings->autoCoverageValue = false;
    }
}