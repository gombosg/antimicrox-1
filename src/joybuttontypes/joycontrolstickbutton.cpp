/* antimicro Gamepad to KB+M event mapper
 * Copyright (C) 2015 Travis Nickles <nickles.travis@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "joycontrolstickbutton.h"

#include "globalvariables.h"
#include "messagehandler.h"
#include "joycontrolstick.h"
#include "joycontrolstickmodifierbutton.h"
#include "setjoystick.h"
#include "event.h"

#include <cmath>

#include <QStringList>
#include <QDebug>


JoyControlStickButton::JoyControlStickButton(JoyControlStick *stick, int index, int originset, SetJoystick *parentSet, QObject *parent) :
    JoyGradientButton(index, originset, parentSet, parent)
{
    qInstallMessageHandler(MessageHandler::myMessageOutput);

    this->stick = stick;
}


JoyControlStickButton::JoyControlStickButton(JoyControlStick *stick, JoyStickDirectionsType::JoyStickDirections index, int originset, SetJoystick *parentSet, QObject *parent) :
    JoyGradientButton(index, originset, parentSet, parent)
{
    qInstallMessageHandler(MessageHandler::myMessageOutput);

    this->stick = stick;
}


QString JoyControlStickButton::getDirectionName() const
{
    qInstallMessageHandler(MessageHandler::myMessageOutput);

    QString label = QString();

    switch(m_index)
    {
    case 1:
        label.append(tr("Up"));
        break;

    case 2:
        label.append(tr("Up")).append("+").append(tr("Right"));
        break;

    case 3:
        label.append(tr("Right"));
        break;

    case 4:
        label.append(tr("Down")).append("+").append(tr("Right"));
        break;

    case 5:
        label.append(tr("Down"));
        break;

    case 6:
        label.append(tr("Down")).append("+").append(tr("Left"));
        break;

    case 7:
        label.append(tr("Left"));
        break;

    case 8:
        label.append(tr("Up")).append("+").append(tr("Left"));
        break;

    }

    return label;
}


QString JoyControlStickButton::getPartialName(bool forceFullFormat, bool displayNames) const
{
    qInstallMessageHandler(MessageHandler::myMessageOutput);

    QString temp = stick->getPartialName(forceFullFormat, displayNames);

    temp.append(": ");

    if (!buttonName.isEmpty() && displayNames)
    {
        if (forceFullFormat)
        {
            temp.append(tr("Button")).append(" ");
        }

        temp.append(buttonName);
    }
    else if (!defaultButtonName.isEmpty())
    {
        if (forceFullFormat)
        {
            temp.append(tr("Button")).append(" ");
        }

        temp.append(defaultButtonName);
    }
    else
    {
        temp.append(tr("Button")).append(" ");
        temp.append(getDirectionName());
    }

    return temp;
}


QString JoyControlStickButton::getXmlName()
{
    qInstallMessageHandler(MessageHandler::myMessageOutput);

    return GlobalVariables::JoyControlStickButton::xmlName;
}

/**
 * @brief Get the distance that an element is away from its assigned
 *     dead zone
 * @return Normalized distance away from dead zone
 */
double JoyControlStickButton::getDistanceFromDeadZone()
{
    qInstallMessageHandler(MessageHandler::myMessageOutput);

    return stick->calculateDirectionalDistance();
}

/**
 * @brief Get the distance factor that should be used for mouse movement
 * @return Distance factor that should be used for mouse movement
 */
double JoyControlStickButton::getMouseDistanceFromDeadZone()
{
    qInstallMessageHandler(MessageHandler::myMessageOutput);

    return stick->calculateMouseDirectionalDistance(this);
}


void JoyControlStickButton::setChangeSetCondition(SetChangeCondition condition, bool passive, bool updateActiveString)
{
    Q_UNUSED(updateActiveString);

    qInstallMessageHandler(MessageHandler::myMessageOutput);

    SetChangeCondition oldCondition = setSelectionCondition;

    if ((condition != setSelectionCondition) && !passive)
    {
        if ((condition == SetChangeWhileHeld) || (condition == SetChangeTwoWay))
        {
            // Set new condition
            emit setAssignmentChanged(m_index, this->stick->getIndex(), setSelection, condition);
        }
        else if ((setSelectionCondition == SetChangeWhileHeld) || (setSelectionCondition == SetChangeTwoWay))
        {
            // Remove old condition
            emit setAssignmentChanged(m_index, this->stick->getIndex(), setSelection, SetChangeDisabled);
        }

        setSelectionCondition = condition;
    }
    else if (passive)
    {
        setSelectionCondition = condition;
    }

    if (setSelectionCondition == SetChangeDisabled)
    {
        setChangeSetSelection(-1);
    }

    if (setSelectionCondition != oldCondition)
    {
        buildActiveZoneSummaryString();
        emit propertyUpdated();
    }
}


int JoyControlStickButton::getRealJoyNumber() const
{
    qInstallMessageHandler(MessageHandler::myMessageOutput);

    return m_index;
}


JoyStickDirectionsType::JoyStickDirections JoyControlStickButton::getDirection() const
{
    qInstallMessageHandler(MessageHandler::myMessageOutput);

    return static_cast<JoyStickDirectionsType::JoyStickDirections>(m_index);
}

/**
 * @brief Set the turbo mode that the button should use
 * @param Mode that should be used
 */
void JoyControlStickButton::setTurboMode(TurboMode mode)
{
    qInstallMessageHandler(MessageHandler::myMessageOutput);

    if (isPartRealAxis())
    {
        currentTurboMode = mode;
    }
}

/**
 * @brief Check if button should be considered a part of a real controller
 *     axis. Needed for some dialogs so the program won't have to resort to
 *     type checking.
 * @return Status of being part of a real controller axis
 */
bool JoyControlStickButton::isPartRealAxis()
{
    qInstallMessageHandler(MessageHandler::myMessageOutput);

    return true;
}


double JoyControlStickButton::getLastAccelerationDistance()
{
    qInstallMessageHandler(MessageHandler::myMessageOutput);

    double temp = stick->calculateLastAccelerationButtonDistance(this);
    return temp;
}


double JoyControlStickButton::getAccelerationDistance()
{
    qInstallMessageHandler(MessageHandler::myMessageOutput);

    double temp = stick->calculateAccelerationDistance(this);
    return temp;
}

/**
 * @brief Generate a string that represents slots that will be activated or
 *     slots that are currently active if a button is pressed
 * @return String of currently applicable slots for a button
 */
QString JoyControlStickButton::getActiveZoneSummary()
{
    qInstallMessageHandler(MessageHandler::myMessageOutput);

    QList<JoyButtonSlot*> tempList;

    tempList.append(getActiveZoneList());
    QString temp = buildActiveZoneSummary(tempList);
    return temp;
}


QString JoyControlStickButton::getCalculatedActiveZoneSummary()
{
    qInstallMessageHandler(MessageHandler::myMessageOutput);

    JoyControlStickModifierButton *tempButton = stick->getModifierButton();
    QString temp = QString();
    QStringList stringlist = QStringList();

    if ((tempButton != nullptr) && tempButton->getButtonState() &&
        tempButton->hasActiveSlots() && getButtonState())
    {
        #ifndef QT_DEBUG_NO_OUTPUT
        qDebug() << "Calculated Active Zone Summary: " << tempButton->getCalculatedActiveZoneSummary();
        #endif

        stringlist.append(tempButton->getCalculatedActiveZoneSummary());
    }

    stringlist.append(JoyButton::getCalculatedActiveZoneSummary());
    temp = stringlist.join(", ");

    #ifndef QT_DEBUG_NO_OUTPUT
    qDebug() << "Returned joined zone: " << temp;
    #endif

    return temp;
}


double JoyControlStickButton::getLastMouseDistanceFromDeadZone()
{
    qInstallMessageHandler(MessageHandler::myMessageOutput);

    return stick->calculateLastMouseDirectionalDistance(this);
}


double JoyControlStickButton::getCurrentSpringDeadCircle()
{
    qInstallMessageHandler(MessageHandler::myMessageOutput);

    double result = (springDeadCircleMultiplier * 0.01);

    if ((m_index == static_cast<int>(JoyControlStick::StickLeft)) || (m_index == static_cast<int>(JoyControlStick::StickRight)))
    {
        result = stick->getSpringDeadCircleX() * (springDeadCircleMultiplier * 0.01);
    }
    else if ((m_index == static_cast<int>(JoyControlStick::StickUp)) || (m_index == static_cast<int>(JoyControlStick::StickDown)))
    {
        result = stick->getSpringDeadCircleY() * (springDeadCircleMultiplier * 0.01);
    }
    else if ((m_index == static_cast<int>(JoyControlStick::StickRightUp)) || (m_index == static_cast<int>(JoyControlStick::StickRightDown)) ||
             (m_index == static_cast<int>(JoyControlStick::StickLeftDown)) || (m_index == static_cast<int>(JoyControlStick::StickLeftUp)))
    {
        result = 0.0;
    }

    return result;
}


JoyControlStick* JoyControlStickButton::getStick() const {

    qInstallMessageHandler(MessageHandler::myMessageOutput);

    return stick;
}
