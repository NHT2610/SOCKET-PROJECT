USE SQLDBQuery
GO

--Tạo bảng lưu trữ tài khoản đăng kỳ từ client
CREATE TABLE Account
(
	Username nvarchar(100),
	Password nvarchar(100)
)
GO

--Thêm tài khoản vào bảng
INSERT dbo.Account (Username, Password)
VALUES (N'toilaTruc', N'26102002')

INSERT dbo.Account (Username, Password)
VALUES (N'sieunhan', N'12@754')

INSERT dbo.Account (Username, Password)
VALUES (N'ninjaflash', N'nvc123@34')

INSERT dbo.Account (Username, Password)
VALUES (N'toidaidot', N'9999#64')

INSERT dbo.Account (Username, Password)
VALUES (N'QuaMonMMT', N'nht12345')

INSERT dbo.Account (Username, Password)
VALUES (N'hellocacban', N'nht20ctt4')

--Xóa account trong bảng
DELETE dbo.Account WHERE Username = 'MangMayTinh'