6a8e935e317519fb88f2d253eb96acf29b83c57f
 sql/updates/world/2014_08_30_01_world_creature_addon.sql | 2 ++
 1 file changed, 2 insertions(+)

diff --git a/sql/updates/world/2014_08_30_01_world_creature_addon.sql b/sql/updates/world/2014_08_30_01_world_creature_addon.sql
new file mode 100644
index 0000000..40ce35a
--- /dev/null
+++ b/sql/updates/world/2014_08_30_01_world_creature_addon.sql
@@ -0,0 +1,2 @@
+--
+DELETE FROM `creature_addon` WHERE `guid`=137619;
