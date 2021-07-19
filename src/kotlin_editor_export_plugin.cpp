#ifdef TOOLS_ENABLED

#include <modules/kotlin_jvm/src/jni/jvm.h>
#include "kotlin_editor_export_plugin.h"
#include "core/os/file_access.h"
#include "logging.h"
#include "gd_kotlin.h"

static constexpr const char* all_jvm_feature{"export-all-jvm"};

void KotlinEditorExportPlugin::_export_begin(const Set<String>& p_features, bool p_debug, const String& p_path,
                                             int p_flags) {
    // Add mandatory jars to pck
    Vector<String> files_to_add;
    bool is_android_export{p_features.has("Android")};
    if (is_android_export) {
        files_to_add.push_back("res://build/libs/main-dex.jar");
        files_to_add.push_back("res://build/libs/godot-bootstrap-dex.jar");
    } else {
        String graal_usercode_lib;
        if (p_features.has("Windows")) {
            graal_usercode_lib = "usercode.dll";
        } else if (p_features.has("OSX")) {
            graal_usercode_lib = "usercode.dylib";
        } else if (p_features.has("X11")) {
            graal_usercode_lib = "usercode.so";
        }
        if (p_features.has(all_jvm_feature)) {
            files_to_add.push_back("res://build/libs/main.jar");
            files_to_add.push_back("res://build/libs/godot-bootstrap.jar");
            files_to_add.push_back(vformat("res://build/libs/%s", graal_usercode_lib));
        } else {
            jni::Jvm::Type jvm_type{GDKotlin::get_instance().get_configuration().get_vm_type()};
            switch (jvm_type) {
                case jni::Jvm::HOTSPOT:
                    files_to_add.push_back("res://build/libs/main.jar");
                    files_to_add.push_back("res://build/libs/godot-bootstrap.jar");
                    break;
                case jni::Jvm::GRAAL:
                    files_to_add.push_back(vformat("res://build/libs/%s", graal_usercode_lib));
                    break;
                default:
                    LOG_ERROR("Unknown VM type, aborting export.")
                    return;
            }
        }
    }
    for (int i = 0; i < files_to_add.size(); ++i) {
        const String& file_to_add{files_to_add[i]};
        add_file(file_to_add, FileAccess::get_file_as_array(file_to_add), false);
    }

    // Copy JRE for desktop platforms
    if (!is_android_export) {
        const Vector<String>& path_split = p_path.split("/");
        String export_dir{p_path.replace(path_split[path_split.size() - 1], "")};
        Error error;
        DirAccess* dir_access{DirAccess::open(export_dir, &error)};
        if (error == OK) {
            if (dir_access->copy_dir(
                    ProjectSettings::get_singleton()->globalize_path("res://jre"),
                    vformat("%s/jre", dir_access->get_current_dir())
            ) != OK) {
                LOG_ERROR("Cannot copy jre folder to export folder, please make sure you created a jre in project "
                          "root folder using jlink.")
            }
        } else {
            LOG_ERROR(vformat("Cannot copy JRE folder to %s, error is %s", p_path, error))
        }
        memdelete(dir_access);
    }
}

#endif
