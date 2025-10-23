object PromptStart: TPromptStart
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Rozpocznij gr'#281
  ClientHeight = 300
  ClientWidth = 400
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  Position = poOwnerFormCenter
  TextHeight = 15
  object Label1: TLabel
    Left = 24
    Top = 24
    Width = 39
    Height = 15
    Caption = 'Plansza'
  end
  object Label2: TLabel
    Left = 24
    Top = 136
    Width = 262
    Height = 15
    Caption = 'Pozycja pustego pola (x, y) (1, 7).    (4, 4) to '#347'rodek'
  end
  object RadioButton1: TRadioButton
    Left = 24
    Top = 56
    Width = 113
    Height = 17
    Caption = 'Nowoczesna'
    Checked = True
    TabOrder = 0
    TabStop = True
  end
  object RadioButton2: TRadioButton
    Left = 24
    Top = 96
    Width = 113
    Height = 17
    Caption = 'Klasyczna'
    TabOrder = 1
  end
  object xInput: TEdit
    Left = 24
    Top = 168
    Width = 81
    Height = 23
    NumbersOnly = True
    TabOrder = 2
    Text = '4'
  end
  object yInput: TEdit
    Left = 111
    Top = 168
    Width = 82
    Height = 23
    NumbersOnly = True
    TabOrder = 3
    Text = '4'
  end
  object OKButton: TButton
    Left = 256
    Top = 232
    Width = 75
    Height = 25
    Cursor = crHandPoint
    Caption = 'Start'
    Default = True
    TabOrder = 4
    OnClick = OKButtonClick
  end
  object CancelButton: TButton
    Left = 24
    Top = 232
    Width = 145
    Height = 25
    Caption = 'Odechcia'#322'o mi si'#281' gra'#263
    TabOrder = 5
    OnClick = CancelButtonClick
  end
end
