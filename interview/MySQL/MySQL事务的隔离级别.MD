MySQL事务
https://juejin.im/entry/57b3da075bbb500063f7b18b

(一)隔离级别
    由低到高为
    Read Uncommitted < ReadCommitted < Repeatable Read < Serializable

(二)读未提交
    读取未提交的数据, 会产生'脏读'

(三)读提交
    大多数数据库系统默认的隔离级别(不是MySQL默认的)
    定义: 一个事务只能看见已经提交事务所做的改变
    会产生的问题: '不可重复读'
    但是同一事务的其他实例在该实例的处理期间可能会有新的commit, 所以同一select可能返回不同的结果

(四)可重复读
    MySQL的默认事务隔离级别
    确保了同一个事务的多个实例在并发读取数据的时候, 会看到同样的数据行
    会产生的问题: '幻读'
    指某个用户读取到某一范围的数据行时, 另一个事务又在该范围内插入了新行, 当用户再读取该范围的数据行时,会发现有新的"幻影"行

(五)序列化执行
    强制事务排序, 使之不能相互冲突, 从而解决了幻读问题
    在每个读的数据行上加共享锁, 会导致大量的锁竞争
