unit uClient;

interface

uses
  System.SysUtils, System.Types, System.UITypes, System.Classes, System.Variants,
  FMX.Types, FMX.Controls, FMX.Forms, FMX.Graphics, FMX.Dialogs,
  IdBaseComponent, IdComponent, IdTCPConnection, IdTCPClient, FMX.Objects,
  FMX.ScrollBox, FMX.Memo, FMX.StdCtrls, FMX.Edit, FMX.Controls.Presentation,
  System.JSON;

type
  TFrmMain = class(TForm)
    GroupBox1: TGroupBox;
    Label1: TLabel;
    Edit1: TEdit;
    Label2: TLabel;
    Edit2: TEdit;
    Label3: TLabel;
    Switch1: TSwitch;
    Memo1: TMemo;
    Rectangle1: TRectangle;
    Circle1: TCircle;
    StyleBook1: TStyleBook;
    IdTCPClient1: TIdTCPClient;
    procedure FormCreate(Sender: TObject);
    procedure Switch1Switch(Sender: TObject);
    procedure Rectangle1MouseMove(Sender: TObject; Shift: TShiftState; X,
      Y: Single);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  FrmMain: TFrmMain;

implementation

{$R *.fmx}

procedure TFrmMain.FormCreate(Sender: TObject);
begin
  Rectangle1.Cursor := crNone;
end;

procedure TFrmMain.Switch1Switch(Sender: TObject);
begin
  try
    if Switch1.IsChecked then
    begin
      IdTCPClient1.Host := Edit1.Text;
      IdTCPClient1.Port := Edit2.Text.ToInteger;
      IdTCPClient1.Connect;
      Memo1.Lines.Add('Client connected!');
    end
    else
    begin
      IdTCPClient1.Disconnect;
      Memo1.Lines.Add('Client disconnected!');
    end;
  except
    IdTCPClient1.Disconnect;
    Memo1.Lines.Add('Oops ... something went wrong!');
  end;
end;

procedure TFrmMain.Rectangle1MouseMove(Sender: TObject; Shift: TShiftState; X,
  Y: Single);
var
  position, request: TJSONObject;
begin
  if (X <= Rectangle1.Width - Circle1.Width) and
     (Y <= Rectangle1.Height - Circle1.Height) then
  begin
    Circle1.Position.X := X;
    Circle1.Position.Y := Y;

    if IdTCPClient1.Connected and IdTCPClient1.Socket.Connected then
    begin
      position := TJSONObject.Create();
      request := TJSONObject.Create();

      try
        position.AddPair(TJSONPair.Create('x', X.ToString));
        position.AddPair(TJSONPair.Create('y', Y.ToString));

        request.AddPair(TJSONPair.Create('position', TJSONObject.ParseJSONValue(position.ToJSON) as TJSONObject));

        IdTCPClient1.Socket.WriteLn(request.ToJSON);
      finally
        position.DisposeOf;
        request.DisposeOf;
      end;
    end;
  end;
end;

end.

