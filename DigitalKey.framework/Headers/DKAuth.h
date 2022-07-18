//
//  DKAuth.h
//  DigitalKey
//
//  Created by SM2 on 2022/3/29.
//
//  TimeoutInterval is 30s
//

#import <Foundation/Foundation.h>
#import <DigitalKey/DKInfoModel.h>
#import <DigitalKey/DKDeviceModel.h>

NS_ASSUME_NONNULL_BEGIN

// 蓝牙硬件状态
typedef NS_ENUM (NSUInteger, BleStatus) {
    BleStatusUnknown      = 0,
    BleStatusResetting    = 1,                /* 蓝牙重置 */
    BleStatusUnsupported  = 2,                /* 不支持蓝牙 */
    BleStatusUnauthorized = 3,                /* 蓝牙未授权 */
    BleStatusPoweredOff   = 4,                /* 手机蓝牙关闭 */
    BleStatusPoweredOn    = 5,                /* 手机蓝牙开启 */
    
    BleStatusNotFound     = 10,               /* 未发现车辆蓝牙设备 */
    BleStatusConnect      = 11,               /* 蓝牙已连接 */
    BleStatusConnectFail  = 12,               /* 蓝牙连接失败 */
    BleStatusDisConnect   = 13,               /* 蓝牙断开连接 */
    BleStatusDisConnectFail = 14,             /* 蓝牙断开连接失败 */
    BleStatusAuthSuccess  = 15,               /* 蓝牙认证成功 */
    BleStatusAuthFail     = 16,               /* 蓝牙认证失败 */
};

// 蓝牙控车命令类型
typedef NS_ENUM (NSUInteger, CommandType) {
    CommandTypeUnknown      = 0,              /* 未知类型 */
    CommandTypeControl      = 1,              /* 控制类型 */
    CommandTypeTransfer     = 2,              /* 透传类型 */
    CommandTypeAuth         = 3,              /* 认证类型 */
    CommandTypeCalibration  = 4,              /* 标定类型 */
    CommandTypeRetry        = 5,              /* 重发类型 */
};

// 蓝牙标定类型
typedef NS_ENUM (NSUInteger, DetermineType) {
    DetermineTypePE = 1,
    DetermineTypePS
};

// 钥匙类型 全部，可用，不可用。
typedef NS_ENUM (NSUInteger, KeyType) {
    KeyTypeAll  = 0,
    KeyTypeValid,
    KeyTypeInvalid
};

// 蓝牙唤醒
typedef NS_ENUM (NSUInteger, RegionType) {
    RegionTypeUnknown,
    RegionTypeInside,
    RegionTypeOutside
};

typedef void(^BleStatusBlock)(BleStatus status);

/* 蓝牙接收数据回调  */
typedef void(^BleReceiveBlock)(CommandType type, NSData *data);

/* 自动唤醒回调 */
typedef void(^BleAutoWeakUpBlock)(RegionType type);

@interface DKAuth : NSObject

/* 蓝牙接收数据回调 */
@property (copy, nonatomic) BleReceiveBlock bleReceive;

/* 蓝牙唤醒数据回调 */
@property (copy, nonatomic) BleAutoWeakUpBlock bleAutoWeakUp;

+ (instancetype)shareInstance;

#pragma mark - 初始化

/// 版本号
- (NSString *)version;

/// 初始化 CA 证书，客户端证书，用户 HTTPS，需要在 initSDK 之前调用
/// @param CACertPath     CA 证书链路径
/// @param clientCertPath 客户端证书路径
/// @param clientCertPwd  客户端证书密码
/// @param requestHeader  Request Header
/// @param verifyServerCertificate 是否校验证书
/// @param verifyDomain   是否校验 Domain
- (void)initCACertPath:(NSString *)CACertPath
        clientCertPath:(NSString *)clientCertPath
         clientCertPwd:(NSString *)clientCertPwd
          requetHeader:(NSDictionary *)requestHeader
verifyServerCertificate:(BOOL)verifyServerCertificate
          verifyDomain:(BOOL)verifyDomain;

/// 初始化 SDK
/// @param userID  用户 ID
/// @param baseURL 根路径
/// @param token   Token
- (void)initSDK:(NSString *)userID baseURL:(NSString *)baseURL token:(NSString *)token;


/// 初始化蓝牙配置
/// @param serviceUUID 蓝牙服务  UUID
/// @param readUUID    蓝牙读特征 UUID
/// @param writeUUID   蓝牙写特征 UUID
- (void)initBleConfig:(NSString *)serviceUUID
             readUUID:(NSString *)readUUID
            writeUUID:(NSString *)writeUUID;

/// 设置当前使用的蓝牙钥匙
- (BOOL)setCurrentDigitalKey:(NSString *)dkID;

/// 获取当前钥匙
- (NSString *)getCurrentDigitalKey;

/// 获取当前设备指纹
- (NSString *)mobileID;

#pragma mark - 蓝牙

/// 当前的蓝牙状态
- (BleStatus)bleStatus;

/// 蓝牙唤醒监听
- (void)monitorAuthWeakup:(BleAutoWeakUpBlock)block;

/// 开启无感开锁
- (BOOL)startScanBleService;

/// 关闭无感开锁
- (BOOL)stopScanBleService;

/// 连接蓝牙
- (void)bleConnect:(void(^)(BleStatus status, NSError * _Nullable error))block;

/// 唤醒蓝牙
- (void)bleWeakUp:(void (^)(BOOL isSuccess, NSError * _Nullable error))block;

/// 蓝牙数据类型
- (CommandType)bleCommandType:(NSData *)data;

/// 开始认证蓝牙
- (NSData *)bleAuthStart;

/// 认证蓝牙解析
- (void)bleAuthParseData:(NSData *)data
                 process:(nullable void (^)(NSData *processData))process
                 success:(nullable void (^)(BOOL isSuccess))success
                 failure:(nullable void (^)(NSError *error))failure;

/// 断开蓝牙连接
- (void)bleDisConnect:(void(^)(BOOL isSuccess, NSError *_Nullable error))block;


/// 封装车控指令
/// @param data           控车指令数据
/// @param permissionMask 控车权限掩码
/// @param type           控车命令类型
- (NSData *)packageInstruction:(NSData *)data
                permissionMask:(NSData * _Nullable )permissionMask
                          type:(CommandType)type;

/// 发送指令
- (void)bleSendData:(NSData *)data;

/// 解析车端指令
- (void)bleParseInstruction:(NSData *)data
                      block:(void (^)(NSData *_Nullable parseData, NSError *_Nullable error))block;


#pragma mark - 标定部分（仅用户标定的 App 进行调用）


/// 上传标定数据
/// @param modeCode  车型编码
/// @param ps        ps Value
/// @param pe        pe Value
/// @param block     成功，错误信息
- (void)uploadDemarcateWithVehicleModeCode:(NSString *)modeCode
                                        PS:(NSString *)ps
                                        PE:(NSString *)pe
                                     block:(void (^)(BOOL isSuccess, NSError *_Nullable error))block;

/// 开始蓝牙标定
- (NSData *)bleDemarcateStart;

/// 设置标定
/// @param type PS / PE
/// @param value 待标定值 ( 开始蓝牙标定返回的 data 数据值)
- (NSData *)bleDemarcateDataWithType:(DetermineType)type value:(NSData *)value;

#pragma mark - 数字钥匙

/// 激活数字钥匙
/// @param vins      车辆信息
/// @param authToken AuthToken
/// @param block     DKID，激活结果，错误信息
- (void)activateDigitalKeyVins:(NSArray  *)vins
                     authToken:(NSString *)authToken
                         block:(void (^)(NSArray * _Nullable ids, BOOL isSuccess, NSError * _Nullable error))block;

/// 下载数字钥匙
/// @param vin       车辆信息
/// @param dkID      钥匙
/// @param authToken AuthToken
/// @param block     下载结果，错误信息
- (void)downloadDigitalKeyWithVin:(NSString *)vin
                             dkID:(NSString *)dkID
                        authToken:(NSString *)authToken
                            block:(void (^)(BOOL isSuccess, NSError * _Nullable error))block;

/// 获取权限配置
/// @param vin             车辆信息
/// @param permissionType  全部 0, 近控 1, 远控 2
/// @param block           权限信息，错误信息。
- (void)permissionConfigWithVin:(NSString *)vin
                 permissionType:(int)permissionType
                          block:(void (^)(NSArray<DKPermissionModel *> *permission,
                                          NSError * _Nullable error))block;

/// 获取分享的次数
/// @param vin        车辆信息
/// @param block      maxAmount: 最大数量，useAmount: 已用数量，available: 可用数量, 错误信息
- (void)shareDigitalKeyAmountWithVin:(NSString *)vin
                               block:(void (^)(NSInteger maxAmount, NSInteger useAmount, NSInteger available,
                                               NSError * _Nullable error))block;

/// 数字钥匙分享
/// @param vin        车辆信息
/// @param permission 权限信息
/// @param startTime  钥匙生效时间
/// @param endTime    钥匙到期时间
/// @param type       钥匙类型（亲友身份1，临时身份2，试乘试驾3，租赁类型4）
/// @param times      数字钥匙可用次数
/// @param recipient  接收者信息
/// @param block      分享结果，错误信息
- (void)shareDigitalKeyWithVin:(NSString *)vin
                    permission:(NSArray<DKPermissionModel *> *)permission
                     startTime:(NSString *)startTime
                       endTime:(NSString *)endTime
                       keyType:(NSInteger)type
                         times:(NSInteger)times
                     recipient:(NSArray *)recipient
                         block:(void (^)(BOOL isSuccess,
                                         NSError * _Nullable error))block;

/// 数字钥匙撤销分享
/// @param vin        车辆信息
/// @param dkID       车辆钥匙 ID
/// @param block      撤销结果，错误信息
- (void)revokeSharedDigitalKeyWithVin:(NSString *)vin
                                 dkID:(NSString *)dkID
                                block:(void (^)(BOOL isSuccess, NSError * _Nullable error))block;

/// 数字钥匙删除
/// @param vin        车辆信息
/// @param dkIDs      车辆钥匙 ID 数组
/// @param block      删除结果，错误信息
- (void)deleteSharedDigitalKeyWithVin:(NSString *)vin
                                dkIDs:(NSArray  *)dkIDs
                                block:(void (^)(BOOL isSuccess, NSError * _Nullable error))block;

/// 获取车主数字钥匙状态
/// @param vin        车辆信息
/// @param block      钥匙状态, 钥匙 ID， 错误信息
- (void)digitalKeyOwnerStatusWithVin:(NSString *)vin
                               block:(void (^)(DigitalKeySatus status,
                                               NSString *dkID,
                                               NSError  *error))block;

/// 获取非车主数字钥匙列表
/// @param vin        车辆信息
/// @param block      钥匙状态，钥匙 ID，错误信息
 - (void)digitalKeyNonOwnerStatusWithVin:(NSString *)vin
                                 pageNum:(NSInteger)pageNum
                                pageSize:(NSInteger)pageSize
                                   block:(void (^)(NSArray<DKInfoModel *> * _Nullable infoList,
                                                   NSError  *error))block;

/// 获取被分享数字钥匙列表（他人分享车主）
/// @param vin        车辆信息，空即为全部
/// @param type       钥匙类型
/// @param pageNum    分页参数，0 开始
/// @param pageSize   每页数量
/// @param block      删除结果，错误信息
- (void)beSharedDigitalKeyListWithVin:(NSString * _Nullable)vin
                              keyType:(KeyType)type
                              pageNum:(NSInteger)pageNum
                             pageSize:(NSInteger)pageSize
                                block:(void (^)(NSArray<DKInfoModel *> * _Nullable infoList, NSError * _Nullable error ))block;

/// 删除被分享数字钥匙列表
/// @param vin    车辆信息
/// @param dkId   钥匙 ID
/// @param block  删除结果，错误信息
- (void)deleteBeSharedDigitalKeyWithVin:(NSString *)vin
                                   dkId:(NSString *)dkId
                                  block:(void (^)(BOOL isSuccess, NSError * _Nullable error))block;


/// 获取车主授权钥匙列表
/// @param vin        车辆信息
/// @param block      授权钥匙列表，错误信息
- (void)authorizationDigitalKeyListWithVin:(NSString *)vin
                                     block:(void (^)(NSArray<DKInfoModel *> * _Nullable infoList, NSError * _Nullable error))block;

/// 获取车主授权钥匙历史
/// @param vin        车辆信息
/// @param pageNum    分页参数，0 开始
/// @param pageSize   每页数量
/// @param block      授权钥匙列表，错误信息了，
- (void)authorizationDigitalKeyHistoryListWithVin:(NSString *)vin
                                          pageNum:(NSInteger)pageNum
                                         pageSize:(NSInteger)pageSize
                                            block:(void (^)(NSArray<DKInfoModel *> * _Nullable infoList, NSError * _Nullable error))block;


/// 获取设备列表
/// 不包含本机设备
/// @param vin        车辆信息
/// @param block      设备列表，错误信息
- (void)deviceListWithVin:(NSString *)vin
                    block:(void (^)(NSArray<DKDeviceModel *> * _Nullable deviceList,
                                    NSError * _Nullable error))block;


/// 注销设备
/// @param mobileId   手机ID
/// @param vin        车辆信息
/// @param block      注销结果，错误信息
- (void)cancelDeviceWithMobileId:(NSString *)mobileId
                             vin:(NSString *)vin
                           block:(void (^)(BOOL isSuccess, NSError * _Nullable error))block;



/// 车辆数字钥匙业务注销
/// @param vin       车辆信息
/// @param authToken 认证密钥
/// @param block     注销结果，错误信息
- (void)cancelDigitalKeyProfessionWithVin:(NSString *)vin
                                authToken:(NSString *)authToken
                                    block:(void (^)(BOOL isSuccess, NSError * _Nullable error))block;

/// 消息推送
/// @param messageStr 推送信息
/// @param authToken  认证密钥
/// @param block      推送处理结果，错误信息
- (void)pushMessageStr:(NSString *)messageStr
             authToken:(NSString *)authToken
                 block:(void (^)(BOOL isSuccess, NSError * _Nullable error))block;


/// 消息推送测试( 测试接口，不作为正式接口使用 )
/// @param commandType 数据类型
/// @param block       测试数据
- (void)pushTestWithCommandType:(NSInteger )commandType
                          block:(void (^)(NSDictionary *_Nullable data, NSError * _Nullable error))block;


#pragma mark - 缓存

// 清除缓存数据 (接口交互时候的密钥数据、随机数等)
- (void)clearCacheData;

// 清除数据库数据及配置信息等数据。退出登录应调用 clearData 方法。
- (void)clearData;

// 清空蓝牙数据接口（测试用）
- (void)clearBleData;

@end

NS_ASSUME_NONNULL_END
